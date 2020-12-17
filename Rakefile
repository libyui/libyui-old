# -*- coding: utf-8 -*-
# make continuous integration using rubygem-packaging_rake_tasks and
# rubygem-libyui-rake.
# Copyright © 2014 SUSE LLC
# MIT license

require "libyui/rake"

Libyui::Tasks.configuration do |conf|
  include Libyui::Tasks::Helpers

  conf.skip_license_check << /^legacy-buildtools\/.*/
  conf.skip_license_check << /^Makefile\.repo$/
  conf.skip_license_check << /^src\/.*\.h\.in$/
  conf.skip_license_check << /^pkgconfig\/.*\.pc\.in$/
  conf.skip_license_check << /CMakeLists\.txt$/
end
