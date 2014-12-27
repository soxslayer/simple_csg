include $(top_srcdir)/Makefile.common

bin_PROGRAMS=simple_csg

simple_csg_SOURCES= \
  bbox_mesh.cpp \
  debug_mesh.cpp \
  drawable.cpp \
  gpu_pipeline.cpp \
  main.cpp \
  marker_mesh.cpp \
  mesh.cpp \
  mesh_builder.cpp \
  shader.cpp \
  tango_palette.cpp \
  volume_mesh.cpp \
  window.cpp

simple_csg_DEPENDENCIES= \
  $(top_builddir)/src/csg/libcsg.a
simple_csg_LDADD= \
  $(top_builddir)/src/csg/libcsg.a \
  $(top_builddir)/src/util/libutil.a

if HAVE_SDL2
  simple_csg_SOURCES+=sdl_window.cpp
  simple_csg_LDADD+=$(SDL2_LIBS) $(GL_LIBS) $(GLEW_LIBS)
  AM_CPPFLAGS+=$(SDL2_CFLAGS) $(GL_CFLAGS) $(GLEW_CFLAGS)
endif

if HAVE_LOAX
  simple_csg_SOURCES+=loax_window.cpp
  simple_csg_LDADD+=$(LOAX_LIBS)
  AM_CPPFLAGS+=$(LOAX_CFLAGS)
endif
