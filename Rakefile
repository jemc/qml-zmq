
require 'qt/commander'
require 'pp'

task :default => :android

task :vendor do
  require_relative 'vendor/build.rb'
  
  Qt::Commander::Creator.profiles
  .select(&:android?).map(&:toolchain).uniq.each do |toolchain|
    toolchain.env do
      vendor_build
    end
  end
end

task :android do
  Qt::Commander::Creator.profiles.select(&:android?).each do |profile|
    profile.toolchain.env do
      system "#{profile.version.qmake} *.pro -spec android-g++" and
      system "make"
    end
  end
end

task :test do
  system "qmake *.pro && make && qmltestrunner"
end

task :clean do
  `make clean && rm Makefile`
end

task :testone do
  testname = "ZReq,ZRouter::test_messages"
  system "qmake *.pro && make && qmltestrunner #{testname}"
end

task :cleantest => [:clean, :test]
