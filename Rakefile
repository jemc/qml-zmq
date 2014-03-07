
task :default => :test

task :test do 
  exec "qmake plugin.pro && make && qmltestrunner"
end

task :clean do
  `make clean && rm Makefile`
end

task :testone do
  testname = "inproc::test_talk_but_no_crosstalk"
  exec "qmake plugin.pro && make && qmltestrunner #{testname}"
end

task :cleantest => [:clean, :test]
