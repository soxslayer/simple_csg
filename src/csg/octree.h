#ifndef __OCTREE__
#define __OCTREE__

#include <stdexcept>

#include "math/vector.h"

namespace CSG
{

template<typename T>
class Octree
{
private:
  struct Item
  {
    T item;
    Math::Vector3<float> position;
  };

  union LeafElement
  {
    Item item;
    Octree* child;

    LeafElement() : child(nullptr) { }
    ~LeafElement() { }
  };

public:
  class Iterator
  {
  public:
    Iterator(Octree<T>* octree)
    {
      _node = octree;

      if (!_node || _node->_n_items == 0) {
        _node = nullptr;
        return; // An empty tree or end Iterator
      }

      _idx_stack.push_front(0);

      next_leaf();
      next();
    }

    Octree<T>* get_node() const
    {
      return _node;
    }

    const Iterator& operator++()
    {
      if (!_node)
        return *this;

      ++_idx_stack.front();

      next();


      return *this;
    }

    T& operator*() const
    {
      if (!_node)
        throw new std::runtime_error("dereferencing null Octree<T>::Iterator");

      return _node->_elements[_idx_stack.front()].item.item;
    }

    T* operator->() const
    {
      if (!_node)
        throw new std::runtime_error("dereferencing null Octree<T>::Iterator");

      return &_node->_elements[_idx_stack.front()].item.item;
    }

    bool operator==(const Iterator& iter) const
    {
      return !_node ? !iter._node
                    : _node == iter._node
                      && _idx_stack.front() == iter._idx_stack.front();
    }

    bool operator!=(const Iterator& iter) const
    {
      return !(*this == iter);
    }

  private:
    std::list<unsigned> _idx_stack;
    Octree<T>* _node;

    void next()
    {
      if (_idx_stack.size() == 0) {
        _node = nullptr;
        return;
      }

      while (_node && _idx_stack.front() >= _node->_n_items) {
        // exhausted all items in this leaf node
        _idx_stack.pop_front();
        _node = _node->_parent;

        if (_idx_stack.size() == 0) {
          _node = nullptr;
          break;
        }

        // traverse up parent links until we find a node we haven't exhausted
        while (_node && _idx_stack.front() >= 7) {
          _idx_stack.pop_front();

          if (_idx_stack.size() == 0)
            _node = nullptr; // exhausted tree
          else
            _node = _node->_parent;
        }

        if (_node) {
          // found one so go to next child
          unsigned idx = ++_idx_stack.front();
          _node = _node->_elements[idx].child;
          _idx_stack.push_front(0);
        }

        next_leaf();
      }
    }

    void next_leaf()
    {
      while (_node && _node->_n_items == _node->_max_items + 1) {
        _idx_stack.push_front(0);
        _node = _node->_elements[0].child;
      }
    }
  };

  class NodeIterator
  {
  public:
    NodeIterator(Octree* node)
      : _node(node)
    {
      add_node(_node);
    }

    const NodeIterator& operator++()
    {
      if (_stack.size() == 0)
        _node = nullptr;
      else {
        _node = _stack.front();
        _stack.pop_front();
        add_node(_node);
      }

      return *this;
    }

    Octree& operator*() const
    {
      if (!_node)
        throw new std::runtime_error("dereferencing null Octree<T>::Iterator");

      return *_node;
    }

    Octree* operator->() const
    {
      if (!_node)
        throw new std::runtime_error("dereferencing null Octree<T>::Iterator");

      return _node;
    }

    bool operator==(const NodeIterator& iter) const
    {
      return _node == iter._node;
    }

    bool operator!=(const NodeIterator& iter) const
    {
      return !(*this == iter);
    }

  private:
    Octree* _node;
    std::list<Octree*> _stack;

    void add_node(Octree* node)
    {
      if (!node)
        return;

      if (node->_n_items > node->_max_items) {
        for (int i = 0; i < 8; ++i)
          _stack.push_back(node->_elements[i].child);
      }
    }
  };

  Octree(float size, int max_items = 8,
         const Math::Vector3<float> center = Math::Vector3<float>::zero())
    : _size(size), _center(center), _n_items(0), _parent(nullptr),
      _child_number(0), _max_items(max_items)
  {
    if (_max_items > 8)
      throw new std::invalid_argument("cannot have more than 8 max items");
  }

  ~Octree()
  {
    if (_n_items == _max_items + 1) {
      for (int i = 0; i < 8; ++i)
        delete _elements[i].child;
    }
  }

  void add_item(const T& item, float x, float y, float z)
  {
    add_item(item, Math::Vector3<float>({ x, y, z }));
  }

  void add_item(const T& item, const Math::Vector4<float>& position)
  {
    add_item(item, position[0], position[1], position[2]);
  }

  void add_item(const T& item, const Math::Vector3<float>& position)
  {
    if (!valid_position(position))
      throw new std::invalid_argument("position outside of octree");

    add_item_internal(item, position);
  }

  Iterator get_neighbor(int x, int y, int z)
  {
    if (_n_items == 0 || _n_items > _max_items)
      throw new std::runtime_error("cannot find a neighbor of non-leaf node");

    std::list<int> neighbor_stack;

    int evec = 0;

    if (x != 0)
      evec |= 1;
    if (y != 0)
      evec |= 2;
    if (z != 0)
      evec |= 4;

    int tvec = 0;

    if (x > 0)
      tvec |= 1;
    if (y > 0)
      tvec |= 2;
    if (z > 0)
      tvec |= 4;

    return get_neighbor(evec, tvec, neighbor_stack);
  }

  float size() const { return _size; }
  const Math::Vector3<float>& center() const { return _center; }

  Iterator begin()
  {
    return Iterator(this);
  }

  Iterator end() const
  {
    return Iterator(nullptr);
  }

  NodeIterator nbegin()
  {
    return NodeIterator(this);
  }

  NodeIterator nend() const
  {
    return NodeIterator(nullptr);
  }

private:
  float _size;
  Math::Vector3<float> _center;
  int _n_items;
  Octree* _parent;
  LeafElement _elements[8];
  int _child_number;
  int _max_items;

  Octree(float size, const Math::Vector3<float> center, Octree* parent, 
         int child_number, int max_items)
    : _size(size), _center(center), _n_items(0), _parent(parent),
      _child_number(child_number), _max_items(max_items)
  {
  }

  Iterator get_neighbor(int evec, int tvec, std::list<int>& neighbor_stack)
  {
    int tindex = evec ^ _child_number;

    if (!evec) {
      // we are decending
      if (neighbor_stack.size() == 0 || _n_items <= _max_items)
        return Iterator(this);

      tindex = neighbor_stack.front();
      neighbor_stack.pop_front();

      if (neighbor_stack.size() == 0)
        return _elements[tindex].child; // no need to descend

      return _elements[tindex].child->get_neighbor(0, 0, neighbor_stack);
    }

    if ((evec & _child_number) == tvec) {
      neighbor_stack.push_front(tindex);

      if (!_parent)
        return Iterator(nullptr);

      return _parent->get_neighbor(evec, tvec, neighbor_stack);
    }

    if (!_parent)
      return Iterator(nullptr);

    if (neighbor_stack.size() == 0)
      return Iterator(_parent->_elements[tindex].child); // first call

    // we've reached the top, not descend
    return _parent->_elements[tindex].child->get_neighbor(0, 0, neighbor_stack);
  }

  unsigned get_child_index(const Math::Vector3<float>& position)
  {
    return get_child_index(position[0], position[1], position[2]);
  }

  unsigned get_child_index(float x, float y, float z)
  {
    unsigned idx = 0;

    if (x > _center[0])
      idx |= 1;
    if (y > _center[1])
      idx |= 2;
    if (z > _center[2])
      idx |= 4;

    return idx;
  }

  Math::Vector3<float>  get_child_center(unsigned child)
  {
    Math::Vector3<float> ret = _center;
    float s4 = _size / 4;

    ret[0] += child & 1 ? s4 : -s4;
    ret[1] += child & 2 ? s4 : -s4;
    ret[2] += child & 4 ? s4 : -s4;

    return ret;
  }

  bool valid_position(const Math::Vector3<float>& position)
  {
    float s2 = _size / 2;

    if (position[0] < _center[0] - s2 || position[0] > _center[0] + s2)
      return false;
    if (position[1] < _center[1] - s2 || position[1] > _center[1] + s2)
      return false;
    if (position[2] < _center[2] - s2 || position[2] > _center[2] + s2)
      return false;

    return true;
  }

  void add_item_internal(const T& item, const Math::Vector3<float>& position)
  {
    if (_n_items < _max_items) {
      for (int i = 0; i < _n_items; ++i) {
        if (position == _elements[i].item.position) {
          _elements[i].item.item = item;
          return;
        }
      }

      _elements[_n_items].item.item = item;
      _elements[_n_items++].item.position = position;
    }
    else {
      if (_n_items == _max_items) {
        Octree* tmp_children[8];
        float s2 = _size / 2;

        for (int i = 0; i < 8; ++i)
          tmp_children[i] = new Octree(s2, get_child_center(i), this, i,
                                       _max_items);

        for (int i = 0; i < _n_items; ++i) {
          tmp_children[get_child_index(_elements[i].item.position)]
              ->add_item_internal(_elements[i].item.item,
                                   _elements[i].item.position);
        }

        for (int i = 0; i < 8; ++i)
          _elements[i].child = tmp_children[i];

        ++_n_items;
      }

      _elements[get_child_index(position)]
          .child->add_item_internal(item, position);
    }
  }
};

} /* namespace CSG */
#endif /* __OCTREE__ */
