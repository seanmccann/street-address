require 'rake/testtask'
require 'rake/extensiontask'

Rake::TestTask.new do |t|
  t.libs << %w(test lib)
  t.pattern = 'test/**/*_test.rb'
end

# Define C extension build task
Rake::ExtensionTask.new('street_address_ext') do |ext|
  ext.lib_dir = 'lib/street_address'
  ext.ext_dir = 'ext/street_address'
end

desc "Run tests"
task :default => [:compile, :test]
