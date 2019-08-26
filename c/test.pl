#!/usr/bin/perl
use 5.16.0;
use warnings FATAL => 'all';

use Test::Simple tests => 22;

system("mkdir -p tmp");

my $prompt = `./build/fuss < /dev/null`;
ok($prompt =~ /fuss\$/, "fuss\$ prompt");

my $inter = `./build/fuss < tests/02-echo-twice.sh`;
ok($inter =~ /fuss\$/, "show prompt interactively");
$inter =~ s/fuss\$//;
ok($inter =~ /fuss\$/, "show prompt interactively twice");
ok($inter =~ /one/, "run interactive command 1");
ok($inter =~ /two/, "run interactive command 2");

my @scripts = glob("tests/*.sh");

for my $script (@scripts) {
    system("rm -f tmp/output");
    system("timeout -k 5 10 ./build/fuss $script > tmp/output");

    my $correct = $script;
    $correct =~ s/\.sh$/.out/;
    my $diff = `diff $correct tmp/output`;

    ok($diff eq "", $script);
    if ($diff ne "") {
        $diff =~ s/^/# /mg;
        print $diff;
    }
}

sub check_errors {
    my ($errs) = @_;
    chomp $errs;
    if ($errs ne "") {
        $errs =~ s/^/# /mg;
        warn $errs;
        return 0;
    }
    else {
        return 1;
    }
}

sub clang_check {
    my $errs = `clang-check -analyze src/*.c -- 2>&1`;
    return check_errors($errs);
}

sub valgrind() {
    system(qq{timeout -k 5 10 valgrind -q --leak-check=full --log-file=build/valgrind.out ./build/fuss < tests/01-echo.sh > build/main.out});
    return `cat -n build/valgrind.out` eq "";
}

ok(clang_check(), "clang check");
ok(valgrind(), "valgrind");

my $procs = `ps | grep memcheck`;
chomp $procs;

ok($procs eq "", "no stray processes after tests");
if ($procs ne "") {
    system("killall memcheck-amd64-");
}
