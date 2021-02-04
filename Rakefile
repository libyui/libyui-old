#--
# Copyright (c) 2015-2019 SUSE LLC
# Copyright (c) 2020-2021 SUSE LLC
#
# make continuous integration using rubygem-libyui-rake.
#
# MIT license
#++
require "libyui/rake"

Libyui::Tasks.configuration do |conf|
  include Libyui::Tasks::Helpers

  conf.skip_license_check << /^legacy-buildtools\/.*/
  conf.skip_license_check << /^Makefile\.repo$/
  conf.skip_license_check << /^src\/.*\.h\.in$/
  conf.skip_license_check << /^pkgconfig\/.*\.pc\.in$/
  conf.skip_license_check << /CMakeLists\.txt$/
end
