#!/usr/bin/env ruby
require 'optparse'
require 'csv'
require_relative 'lister'

params = ARGV.getopts('h', 'input:', 'k:')

if params['h'] || !params['input']
    STDERR.puts "Usage: #{__FILE__} --input counted.csv -k k_items(default: 10)"
    exit 1
end

k = params["k"].empty? ? 10 : params["k"].to_i

lister = Lister.new(params['input'])
lister.read
lister.calc_so_scores
lister.output

p lister.best_k(k)
p lister.worst_k(k)
