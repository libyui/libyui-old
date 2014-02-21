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
  conf.obs_project = "devel:libraries:libyui"
  conf.obs_sr_project = "openSUSE:Factory"
  conf.package_name = Dir.glob("*.spec.in").first.chomp(".spec.in")
  conf.package_dir = "#{BUILDDIR}/package"

  conf.skip_license_check << /bootstrap.sh|ChangeLog|Makefile.cvs/
  conf.skip_license_check << /^buildtools\/.*/
  conf.skip_license_check << /\.(cmake|gv|ui|xpm)$/
  conf.skip_license_check << /^src\/lang_fonts$/
end

desc 'Pretend to run the test suite'
task :test do
  puts 'No tests yet' if verbose
end

LIBYUI_PREFIX = ENV["HOME"] + "/libyui-prefix"
LIBYUI_BASE = ENV.fetch("LIBYUI_BASE", "../libyui")

# build the gem package
desc 'Build a tarball for OBS'
task :tarball do
  rm_rf BUILDDIR
  ln_sf "#{LIBYUI_BASE}/buildtools/CMakeLists.common", "CMakeLists.txt"
  lib_dir = `rpm --eval '%{_lib}'`.chomp
  mkdir_p BUILDDIR
  chdir BUILDDIR do
    # unfortunately buildtools are set up in such a way that
    # for making a package, all dependencies need to be present and installed :-/
    sh("cmake",
       "-DCMAKE_BUILD_TYPE=RELEASE",
       "-DLIB_DIR=#{lib_dir}",
       "-DPREFIX=#{LIBYUI_PREFIX}",
       "-DCMAKE_PREFIX_PATH=#{LIBYUI_PREFIX}",
       "-DENABLE_WERROR=OFF", # gtk needs this
       "..")
    sh "make clean"
    sh "make"
    sh "make install"
    sh "make srcpackage"
  end
end
