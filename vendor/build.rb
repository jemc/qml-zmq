
require_relative 'build_env'

def vendor_exec str
  print "\033[1;33m" # Use bold yellow text color in terminal
  puts str           # Print the command to be executed
  print "\033[0m"    # End terminal coloring
  system str         # Execute the command
end

def vendor_build
  vendor_build_env
  
  # The set of dependency libs of this project, with relevant URLs and options
  libs = [
    [:libsodium,
      # retrieve: [:tar, "https://download.libsodium.org/libsodium/releases/LATEST.tar.gz"],
      retrieve: [:git, "https://github.com/jedisct1/libsodium.git"],
      configure: {
        other: '--disable-soname-versions'
      }
    ],
    [:libzmq,
      # retrieve: [:tar, "http://download.zeromq.org/zeromq-4.0.3.tar.gz"],
      retrieve: [:git, "https://github.com/zeromq/zeromq4-x.git"],
      configure: {
        CPPFLAGS: "-Wno-long-long",
        APP_STL:  :stlport_static,
        # other: '--with-pgm',
      },
    ],
  ]
  
  # Path where sources are to be stored and built, relative to invoking Rakefile
  cache_path = "./vendor/cache/#{ENV['TOOLCHAIN_NAME']}/"
  
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
      vendor_exec "cd #{cache_path}"\
         " && git clone #{url} #{name}"
      vendor_exec "cd #{path}"\
         " && if [ ! -f configure ]; then ./autogen.sh; fi"
    when :tar
      tarfile = "#{name}.tar.gz"
      vendor_exec "cd #{cache_path}"\
         " && if [ ! -f #{tarfile} ]; then wget #{url} -O #{tarfile}; fi"\
         " && mkdir #{name}"\
         " && tar -C #{name} -xvf #{tarfile} --strip=1"
    else
      raise NotImplementedError
    end
    
    exit 1 unless vendor_exec\
     "export PATH=#{ENV['TOOLCHAIN_PATH']}:$PATH"\
     " && cd #{path}"\
     " && ./configure #{configure_flags(lib_opts[:configure])}"\
     " && make"\
     " && make install"\
     " && echo '\n\n'"
  }
  
end
