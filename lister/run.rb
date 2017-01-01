#!/usr/bin/env ruby
require 'optparse'
require 'csv'
require 'pp'
require_relative 'lister'

params = ARGV.getopts('', 'input:', 'k:', 'min-occurrence:')

if !params['input']
    STDERR.puts "Usage: #{__FILE__} --input counted.csv -k (default: 10) --min-occurence (default: 1)"
    exit 1
end

input = params['input']
k = params["k"].nil? ? 10 : params["k"].to_i
m = params["min-occurrence"].nil? ? 1 : params["min-occurrence"].to_i

STDERR.puts "input file: #{input}"
STDERR.puts "k: #{k}"
STDERR.puts "min occurrence: #{m}"

lister = Lister.new(input)
lister.min_oc = m

STDERR.puts "reading #{input}..."
lister.read
STDERR.puts "read!"

STDERR.puts "calculating so-scores..."
lister.calc_so_scores
STDERR.puts "calculated!"

# lister.output

STDERR.puts
STDERR.puts "===== output ====="
puts "adjective: #{lister.adjective} <-> antonym: #{lister.antonym}"
puts "tag: #{lister.tag}"
puts "total_words: #{lister.total_words}, vocabulary: #{lister.vocabulary}"
puts "pos_occurrences: #{lister.pos_oc}, neg_occurrences: #{lister.neg_oc}"
puts
puts "ignored: #{lister.ignored}"
puts "best  K: "
pp lister.best_k(k)
puts "worst K: "
pp lister.worst_k(k)
