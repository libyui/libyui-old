# -*- coding: utf-8 -*-
# make continuous integration using rubygem-packaging_rake_tasks
# Copyright © 2014 SUSE
# MIT license

require "packaging"

# @param s   [String] '... SET( VERSION_MAJOR "3") ...'
# @param key [String] 'VERSION_MAJOR'
# @return "3"
def cmake_value(s, key)
  e_key = Regexp.escape(key)
  m = /SET\s*\(\s*#{e_key}\s+"([^"]*)"\s*\)/.match(s)
  m[1]
end

VERSION_CMAKE = "VERSION.cmake" # filename

# @return [String] like "1.2.3"
def cmake_version
  f = File.read(VERSION_CMAKE)
  cmake_value(f, "VERSION_MAJOR") + "." +
    cmake_value(f, "VERSION_MINOR") + "." +
    cmake_value(f, "VERSION_PATCH")
end

# @return [String] like "1.2.3"
def spec_version(spec_filename)
  File.read(spec_filename)[/^Version:\s*(\S+)/, 1]
end

Packaging.configuration do |conf|
  if ENV["LIBYUI_SUBMIT"] == "SLES"
    conf.obs_api        = "https://api.suse.de/"
    conf.obs_project    = "Devel:YaST:Head"
    conf.obs_target     = "SLE-12-SP1"
    conf.obs_sr_project = "SUSE:SLE-12-SP1:GA"
  else
    conf.obs_project    = "devel:libraries:libyui"
    conf.obs_sr_project = "openSUSE:Factory"
  end

  conf.version = cmake_version

  conf.skip_license_check << /.*/ if conf.package_name =~ /gtk|bindings/
  conf.skip_license_check << /bootstrap.sh|ChangeLog|Makefile.cvs/
  conf.skip_license_check << /^buildtools\/.*/
  conf.skip_license_check << /\.(cmake|gv|ui|xpm)$/
  conf.skip_license_check << /^src\/lang_fonts$/
  conf.skip_license_check << /\.mng$/ # binary
end

namespace :version do
    desc "Check that the version numbers are in sync"
    task :check do
      cmake_v = cmake_version
      Dir.glob("package/*.spec").each do |spec_filename|
        spec_v = spec_version(spec_filename)
        if cmake_v != spec_v
          raise "Version mismatch, #{VERSION_CMAKE}:#{cmake_v} #{spec_filename}:#{spec_v}"
        end
      end
      puts cmake_v if verbose
    end

    desc "Increase the last part of version in spec and cmake files"
    task :bump => :check do
      v = cmake_version.split(".")  # ["1", "2", "3"]

      patch = v.last.to_i.next.to_s # "4"
      s = File.read(VERSION_CMAKE)
      s.sub!(/(^SET.*VERSION_PATCH.*)"([^"\n])*"/, "\\1\"#{patch}\"")
      File.write(VERSION_CMAKE, s)

      v[-1] = patch                 # ["1", "2", "4"]
      Dir.glob("package/*.spec").each do |spec_filename|
        s = File.read(spec_filename)
        s.gsub!(/^\s*Version:.*$/, "Version:        #{v.join '.'}")
        File.write(spec_filename, s)
      end
    end
end

desc 'Pretend to run the test suite'
task :test do
  puts 'No tests yet' if verbose
end
