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

# build the gem package
desc 'Build a tarball for OBS'
task :tarball do
  sh "make -f Makefile.cvs"
  chdir BUILDDIR do
    sh "make clean"
#    sh "make"                   # river jenkins does it; should not be needed
    sh "make srcpackage"
  end
end
