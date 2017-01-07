#!/usr/bin/env ruby
require 'optparse'
require 'csv'
require 'pp'
require_relative 'lister'

params = ARGV.getopts('', 'input:', 'k:10', 'min-occurrence:1')

if !params['input']
    STDERR.puts "Usage: #{__FILE__} --input counted.csv -k (default: 10) --min-occurrence (default: 1)"
    exit 1
end

input = params['input']
k = params["k"].to_i
m = params["min-occurrence"].to_i

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
STDERR.puts "adjective: #{lister.adjective} <-> antonym: #{lister.antonym}"
STDERR.puts "tag: #{lister.tag}"
STDERR.puts "total_words: #{lister.total_words}, vocabulary: #{lister.vocabulary}"
STDERR.puts "pos_occurrences: #{lister.pos_oc}, neg_occurrences: #{lister.neg_oc}"
STDERR.puts
STDERR.puts "K: #{k}"
STDERR.puts "ignored: #{lister.ignored}"

STDERR.puts "best  K: "
PP.pp lister.best_k(k), STDERR

STDERR.puts "worst K: "
PP.pp lister.worst_k(k), STDERR

# pass the results to the standard output
k_best  = lister.best_k(k).map {|adj,_| adj}.join(",")
k_worst = lister.worst_k(k).map {|adj,_| adj}.join(",")

STDERR.puts "write the results to the standard output: "
print "#{k_best} #{k_worst}"
