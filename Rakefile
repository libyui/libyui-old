# -*- coding: utf-8 -*-
# make continuous integration using rubygem-packaging_rake_tasks
# Copyright © 2014 SUSE
# MIT license

require "packaging/tasks"
require "packaging/configuration"
# skip 'tarball' task, it's redefined here
Packaging::Tasks.load_tasks(:exclude => ["tarball.rake"])

BUILDDIR = "build"

Packaging.configuration do |conf|
  spec_template = Dir.glob("*.spec.{in,cmake}").first

  if ENV["LIBYUI_SUBMIT"] == "SLES"
    conf.obs_api        = "https://api.suse.de/"
    conf.obs_project    = "Devel:YaST:Head"
    conf.obs_target     = "SLE-12-SP1"
    conf.obs_sr_project = "SUSE:SLE-12-SP1:GA"
  else
    conf.obs_project    = "devel:libraries:libyui"
    conf.obs_sr_project = "openSUSE:Factory"
  end
  conf.package_name = spec_template[/(.*)\.spec\..*/, 1]
  conf.package_dir  = "#{BUILDDIR}/package"

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

LIBYUI_PREFIX = ENV.fetch("LIBYUI_PREFIX", ENV["HOME"] + "/libyui-prefix")
LIBYUI_BASE   = ENV.fetch("LIBYUI_BASE",   "../libyui")

desc 'Build a tarball for OBS'
task :tarball do
  rm_rf BUILDDIR
  unless File.file? "CMakeLists.txt"
    ln_sf "#{LIBYUI_BASE}/buildtools/CMakeLists.common", "CMakeLists.txt"
  end
  lib_dir = `rpm --eval '%{_lib}'`.chomp
  mkdir_p BUILDDIR
  chdir BUILDDIR do
    # unfortunately buildtools are set up in such a way that
    # for making a package, all dependencies need to be present and installed :-/
    sh("cmake",
       "-DCMAKE_BUILD_TYPE=RELEASE",
       "-DLIB_DIR=#{lib_dir}",
       "-DYPREFIX=#{LIBYUI_PREFIX}",
       "-DCMAKE_PREFIX_PATH=#{LIBYUI_PREFIX}",
       "-DENABLE_WERROR=OFF", # gtk needs this
       "..")
    sh "make clean"
    sh "make -j$(nproc) VERBOSE=1"
    sh "make install"
    sh "make srcpackage"
  end
end
