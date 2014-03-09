
QT_ROOT_SET=ENV['QT_ROOT_SET'] # Should be something like $HOME/Qt5.2.0/5.2.0
raise "Please set the QT_ROOT_SET environment variable" unless QT_ROOT_SET


CUSTOM_PREFIX="#{QT_ROOT_SET}/android_armv7"

HOST = "arm-linux-androideabi"
NDK_ROOT = ENV['ANDROID_NDK_ROOT']
raise "Please set the ANDROID_NDK_ROOT environment variable" unless NDK_ROOT

SYSROOT = "#{NDK_ROOT}/platforms/android-8/arch-arm"
TOOLPATH = "#{NDK_ROOT}/toolchains/#{HOST}-4.7/prebuilt/linux-x86_64/bin"

CPP    = "#{HOST}-cpp"
CC     = "#{HOST}-gcc"
CXX    = "#{HOST}-g++"
LD     = "#{HOST}-ld"
AS     = "#{HOST}-as"
AR     = "#{HOST}-ar"
RANLIB = "#{HOST}-ranlib"

CFLAGS   = "--sysroot=#{SYSROOT}"
CPPFLAGS = "--sysroot=#{SYSROOT}"
CXXFLAGS = "--sysroot=#{SYSROOT}"
LDFLAGS  = ""
LIBS     = "-lc -lgcc -ldl"


# Construct load flags from global variables and library-specific option hash
def configure_flags(opts)
  opts ||= {}
  
  case opts[:APP_STL]
  when :stlport_static
    (opts[:LDFLAGS]  ||= "") << " -L#{NDK_ROOT}/sources/cxx-stl/stlport/libs/armeabi"
    (opts[:CFLAGS]   ||= "") << " -I#{NDK_ROOT}/sources/cxx-stl/stlport/stlport"
    (opts[:CPPFLAGS] ||= "") << " -I#{NDK_ROOT}/sources/cxx-stl/stlport/stlport"
    (opts[:CXXFLAGS] ||= "") << " -I#{NDK_ROOT}/sources/cxx-stl/stlport/stlport"
    (opts[:LIBS]     ||= "") << " -lstlport_static"
  end
  
  [
    "CPP=#{CPP}",
    "CC=#{CC}",
    "CXX=#{CXX}",
    "LD=#{LD}",
    "AS=#{AS}",
    "AR=#{AR}",
    "RANLIB=#{RANLIB}",
    
    "CFLAGS='#{CFLAGS} #{opts[:CFLAGS]} -I#{CUSTOM_PREFIX}/include'",
    "CPPFLAGS='#{CPPFLAGS} #{opts[:CPPFLAGS]} -I#{CUSTOM_PREFIX}/include'",
    "CXXFLAGS='#{CXXFLAGS} #{opts[:CXXFLAGS]} -I#{CUSTOM_PREFIX}/include'",
    "LDFLAGS='#{LDFLAGS} #{opts[:LDFLAGS]} -L#{CUSTOM_PREFIX}/lib'",
    "LIBS='#{LIBS} #{opts[:LIBS]}'",
    
    " --host=#{HOST}",
    " --prefix=#{CUSTOM_PREFIX}",
  ].join(' ')
end
