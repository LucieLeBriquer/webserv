#!/usr/local/bin/perl

print "Content-type: text/html\n\n";
print "<pre>\n";

foreach $key (sort keys(%ENV)) {
  print "$key = $ENV{$key}<p>\n";
}
print "</pre>\n";