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

  f = File.read "VERSION.cmake"
  conf.version =
    cmake_value(f, "VERSION_MAJOR") + "." +
    cmake_value(f, "VERSION_MINOR") + "." +
    cmake_value(f, "VERSION_PATCH")

  conf.skip_license_check << /.*/ if conf.package_name =~ /gtk|bindings/
  conf.skip_license_check << /bootstrap.sh|ChangeLog|Makefile.cvs/
  conf.skip_license_check << /^buildtools\/.*/
  conf.skip_license_check << /\.(cmake|gv|ui|xpm)$/
  conf.skip_license_check << /^src\/lang_fonts$/
  conf.skip_license_check << /\.mng$/ # binary
end

desc 'Pretend to run the test suite'
task :test do
  puts 'No tests yet' if verbose
end
