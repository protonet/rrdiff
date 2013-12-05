require 'mkmf'

if have_library('rsync')
  have_header('librsync.h')
else
  unless File.exists? "librsync-0.9.7/librsync.la"
    `cd librsync-0.9.7; ./configure --enable-static --disable-shared && make librsync.la`
  end

  $CFLAGS << " -Ilibrsync-0.9.7 "
  $LDFLAGS << " #{File.dirname(__FILE__)}/librsync-0.9.7/.libs/librsync.a "
end

create_makefile('rrdiff/rrdiff')
