
host = "arm-linux-androideabi"
ndk_root = ENV['ANDROID_NDK_ROOT']
raise "Please set the ANDROID_NDK_ROOT environment variable" unless ndk_root

sysroot = "#{ndk_root}/platforms/android-8/arch-arm"
incl_path = "#{sysroot}/usr/include"
toolpath = "#{ndk_root}/toolchains/#{host}-4.7/prebuilt/linux-x86_64/bin"


configure_flags = [
  "CPP=#{host}-cpp",
  "CC=#{host}-gcc",
  "CXX=#{host}-g++",
  "LD=#{host}-ld",
  "AS=#{host}-as",
  "AR=#{host}-ar",
  "RANLIB=#{host}-ranlib",
  
  "CFLAGS=--sysroot=#{sysroot}",
  "CPPFLAGS=--sysroot=#{sysroot}",
  "CXXFLAGS=--sysroot=#{sysroot}",
  
  "--host=#{host}",
  "",
].join(' ')


libs = {
  libzmq: {
    retrieve: [:tar, "http://download.zeromq.org/zeromq-4.0.3.tar.gz"],
  }
}

cache_path = "./vendor/cache/"

# Create the cache_path directory
system "mkdir -p #{cache_path}"

# Process each library
libs.each { |name, lib_opts|
  
  # Get retrieve options
  type, url = lib_opts.fetch :retrieve
  
  # Retrieve the source using the given strategy to given path
  path = cache_path + name.to_s
  case type
  when :git
    system "git clone #{url} #{path}"
  when :tar
    tarfile = "#{name}.tar.gz"
    system "cd #{cache_path}"\
       " && if [ ! -f #{tarfile} ]; then wget #{url} -O #{tarfile}; fi"\
       " && mkdir #{name}"\
       " && tar -C #{name} -xvf #{tarfile} --strip=1"
  else
    raise NotImplementedError
  end
  
  system "export PATH=#{toolpath}:$PATH"\
     " && cd #{path}"\
     " && ./configure #{configure_flags} #{lib_opts[:configure]}"\
     " && make"
}
