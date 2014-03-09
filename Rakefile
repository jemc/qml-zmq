
task :default => :android

task :vendor do
  require_relative 'vendor/build.rb'
end

task :android do
  QT_ROOT_SET=ENV['QT_ROOT_SET'] # Should be something like $HOME/Qt5.2.0/5.2.0
  raise "Please set the QT_ROOT_SET environment variable" unless QT_ROOT_SET
  
  system "export ANDROID_NDK_TOOLCHAIN_VERSION=4.7"\
    " && #{ENV['QT_ROOT_SET']}/android_armv7/bin/qmake -spec android-g++ ./plugin.pro"\
    " && make && make install"
end

task :test do
  system "qmake plugin.pro && make && qmltestrunner"
end

task :clean do
  `make clean && rm Makefile`
end

task :testone do
  testname = "inproc::test_talk_but_no_crosstalk"
  system "qmake plugin.pro && make && qmltestrunner #{testname}"
end

task :cleantest => [:clean, :test]
