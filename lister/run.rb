#!/usr/bin/env ruby
require 'optparse'
require 'csv'
require 'pp'
require 'fileutils'
require_relative 'lister'

params = ARGV.getopts('', 'input:', 'k:10', 'min-occurrence:1', 'output-path:', 'black-list:')

if !params['input']
    STDERR.puts "Usage: #{__FILE__} --input counted.csv -k (default: 10) --min-occurrence (default: 1) [--output-path file] [--black-list list]"
    STDERR.puts "e.g., #{__FILE__} -i counted.csv -k 20 -m 5 -o ./out"
    exit 1
end

input = params['input']
k = params["k"].to_i
m = params["min-occurrence"].to_i

STDERR.puts "input file: #{input}"
STDERR.puts "k: #{k}"
STDERR.puts "min occurrence: #{m}"

black_list = nil
unless params["black-list"].nil?
    black_list = File.open(params["black-list"]).read.split("\n")
end

lister = Lister.new(input, black_list)
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
best_k = lister.best_k(k)
PP.pp best_k, STDERR

STDERR.puts "worst K: "
worst_k = lister.worst_k(k)
PP.pp worst_k, STDERR


# output {{{

unless params["output-path"].nil?
    out = params["output-path"]

    BEST_PATH  = "best_#{k}_m#{m}.txt"
    WORST_PATH = "worst_#{k}_m#{m}.txt"

    FileUtils.mkdir_p(out)
    STDERR.puts "output_path:   best  -> #{out}/#{BEST_PATH}"
    STDERR.puts "               worst -> #{out}/#{WORST_PATH}"

    File.write("#{out}/#{BEST_PATH}",  best_k.map{|k,_| k}.join("\n"))
    File.write("#{out}/#{WORST_PATH}", worst_k.map{|k,_| k}.join("\n"))
end
# /output }}}
