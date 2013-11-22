require 'mkmf'

unless File.exists? "librsync-0.9.7/librsync.la"
  `cd librsync-0.9.7; ./configure --enable-static --disable-shared && make librsync.la`
end

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

$LDFLAGS << "librsync-0.9.7/.libs/librsync.a"

HEADER_DIRS = [
  'librsync-0.9.7',
  '/usr/local/include',
  INCLUDEDIR,
  '/usr/include',
]

LIB_DIRS = [
  'librsync-0.9.7/.libs',
  '/usr/local/lib',
  LIBDIR,
  '/usr/lib',
]

dir_config('rrdiff', HEADER_DIRS, LIB_DIRS)

have_header('librsync.h')

create_makefile('rrdiff/rrdiff')
