
task :default => :test

task :test do 
  exec "qmake plugin.pro && make && qmltestrunner"
end

task :clean do
  `make clean && rm Makefile`
end

task :cleantest => [:clean, :test]
