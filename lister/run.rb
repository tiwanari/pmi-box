#!/usr/bin/env ruby
require 'optparse'
require 'csv'
require 'pp'
require_relative 'lister'

params = ARGV.getopts('h', 'input:', 'k:', 'min-occurrence:')

if params['h'] || !params['input']
    STDERR.puts "Usage: #{__FILE__} --input counted.csv -k (default: 10) --min-occurence (default: 1)"
    exit 1
end

input = params['input']
k = params["k"].nil? ? 10 : params["k"].to_i
m = params["min-occurrence"].nil? ? 1 : params["min-occurrence"].to_i

STDERR.pp "input file: #{input}"
STDERR.pp "k: #{k}"
STDERR.pp "min occurrence: #{m}"

lister = Lister.new(input)
lister.min_oc = m

STDERR.pp "reading #{input}..."
lister.read
STDERR.pp "read!"

STDERR.pp "calculating so-scores..."
lister.calc_so_scores
STDERR.pp "calculated!"

# lister.output
pp "ignored: #{lister.ignored}"
pp "best  K: "
pp lister.best_k(k)
pp "worst K: "
pp lister.worst_k(k)
