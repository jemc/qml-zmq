
def vendor_build_env
  
  host = ENV['TOOLCHAIN_HOST']
  raise "Please set the TOOLCHAIN_HOST environment variable" unless host
  
  arch = ENV['TOOLCHAIN_ARCH']
  raise "Please set the TOOLCHAIN_ARCH environment variable" unless arch
  
  ndk_root = ENV['ANDROID_NDK_ROOT']
  raise "Please set the ANDROID_NDK_ROOT environment variable" unless ndk_root
  
  toolpath = ENV['TOOLCHAIN_PATH']
  raise "Please set the TOOLCHAIN_PATH environment variable" unless toolpath
  
  sysroot = "#{ndk_root}/platforms/android-9/arch-#{arch}"
  
  raise "Please set the TOOLCHAIN_NAME environment variable" unless ENV['TOOLCHAIN_NAME']
  prefix_dir = File.join(File.dirname(__FILE__),'prefix/'+ENV['TOOLCHAIN_NAME']) # vendor/prefix
  system "mkdir -p #{prefix_dir}"
  
  _CPP    = "#{host}-cpp"
  _CC     = "#{host}-gcc"
  _CXX    = "#{host}-g++"
  _LD     = "#{host}-ld"
  _AS     = "#{host}-as"
  _AR     = "#{host}-ar"
  _RANLIB = "#{host}-ranlib"
  
  _CFLAGS   = "--sysroot=#{sysroot}"
  _CPPFLAGS = "--sysroot=#{sysroot}"
  _CXXFLAGS = "--sysroot=#{sysroot}"
  _LDFLAGS  = ""
  _LIBS     = "-lc -lgcc -ldl"
  
  
  # Construct load flags from global variables and library-specific option hash
  define_method :configure_flags do |opts|
    opts ||= {}
    
    case opts[:APP_STL]
    when :stlport_static
      case arch
      when 'arm'
        (opts[:LDFLAGS]  ||= "") << " -L#{ndk_root}/sources/cxx-stl/stlport/libs/armeabi"
      when 'x86'
        (opts[:LDFLAGS]  ||= "") << " -L#{ndk_root}/sources/cxx-stl/stlport/libs/x86"
      when 'mips'
        (opts[:LDFLAGS]  ||= "") << " -L#{ndk_root}/sources/cxx-stl/stlport/libs/mips"
      end
      (opts[:CFLAGS]   ||= "") << " -I#{ndk_root}/sources/cxx-stl/stlport/stlport"
      (opts[:CPPFLAGS] ||= "") << " -I#{ndk_root}/sources/cxx-stl/stlport/stlport"
      (opts[:CXXFLAGS] ||= "") << " -I#{ndk_root}/sources/cxx-stl/stlport/stlport"
      (opts[:LIBS]     ||= "") << " -lstlport_static"
    end
    
    [
      "CPP=#{_CPP}",
      "CC=#{_CC}",
      "CXX=#{_CXX}",
      "LD=#{_LD}",
      "AS=#{_AS}",
      "AR=#{_AR}",
      "RANLIB=#{_RANLIB}",
      
      "CFLAGS='#{_CFLAGS} #{opts[:CFLAGS]} -I#{prefix_dir}/include'",
      "CPPFLAGS='#{_CPPFLAGS} #{opts[:CPPFLAGS]} -I#{prefix_dir}/include'",
      "CXXFLAGS='#{_CXXFLAGS} #{opts[:CXXFLAGS]} -I#{prefix_dir}/include'",
      "LDFLAGS='#{_LDFLAGS} #{opts[:LDFLAGS]} -L#{prefix_dir}/lib'",
      "LIBS='#{_LIBS} #{opts[:LIBS]}'",
      
      " --host=#{host}",
      " --prefix=#{prefix_dir}",
    ].join(' ')
  end
  
end
