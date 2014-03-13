
require_relative 'build_env.rb'

# The set of dependency libs of this project, with relevant URLs and options
libs = [
  # [:libsodium,
  #   retrieve: [:tar, "https://download.libsodium.org/libsodium/releases/LATEST.tar.gz"],
  # ],
  [:libzmq,
    # retrieve: [:tar, "http://download.zeromq.org/zeromq-4.0.3.tar.gz"],
    retrieve: [:git, "http://github.com/zeromq/zeromq4-x.git"],
    configure: {
      CPPFLAGS: "-Wno-long-long",
      APP_STL:  :stlport_static,
    },
  ],
]

# Path where sources are to be stored and built, relative to invoking Rakefile
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
    system "cd #{cache_path}"\
       " && git clone #{url} #{name}"
    system "cd #{path}"\
       " && if [ ! -f configure ]; then ./autogen.sh; fi"
  when :tar
    tarfile = "#{name}.tar.gz"
    system "cd #{cache_path}"\
       " && if [ ! -f #{tarfile} ]; then wget #{url} -O #{tarfile}; fi"\
       " && mkdir #{name}"\
       " && tar -C #{name} -xvf #{tarfile} --strip=1"
  else
    raise NotImplementedError
  end
  
  exit 1 unless system\
   "export PATH=#{TOOLPATH}:$PATH"\
   " && cd #{path}"\
   " && ./configure #{configure_flags(lib_opts[:configure])}"\
   " && make"\
   " && make install"\
   " && echo '\n\n'"
}
