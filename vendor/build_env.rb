
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
    
    "CFLAGS='#{CFLAGS} #{opts[:CFLAGS]}'",
    "CPPFLAGS='#{CPPFLAGS} #{opts[:CPPFLAGS]}'",
    "CXXFLAGS='#{CXXFLAGS} #{opts[:CXXFLAGS]}'",
    "LDFLAGS='#{LDFLAGS} #{opts[:LDFLAGS]}'",
    "LIBS='#{LIBS} #{opts[:LIBS]}'",
    
    "--host=#{HOST}",
    "",
  ].join(' ')
end
