use strict;
use warnings;


my @arr=qw/ cstl_count cstl_count_if/;
my @check=qw/ 0 0 /;
my @ret=qw/ 0 0 /;
my @passthru=qw/ 0 1 /;
for(my $t=0;$t<=$#arr;$t++){
	my $name=$arr[$t];
	
	print "#define $name(C,...) ";

	if($ret[$t]==1){
		print "({void* _() {";
	}
	print "_linux_$name(C,__VA_ARGS__, 5, 4, 3, 2, 1, 0)";
	print "(C";
	if($check[$t]==1){
		print(",ARGN(__VA_ARGS__)");
	}
	print ",__VA_ARGS__)";
	if($ret[$t]==1){
		print "}_;})();";
	}
	print "\n";
	print "#define _linux_$name(C,_1, _2, _3, _4, _5, N, ...) _$name ## _ ## N","\n";
	for(my $i=0;$i<=5;$i++){
		print "#define _$name"."_$i";
		print "(C";
		if($check[$t]==1){
			print ",argc";
		}
		for(my $j=1;$j<=$i;$j++){
			print ",_$j";
		}
		
		print ")    (({";
		
		print "_CSTL_TYPEOF(&C) __0=&C;";

		if($passthru[$t]!=1){
			for(my $j=1;$j<=$i;$j++){
				print "_CSTL_TYPEOF(_$j) __$j=_$j;";
			}
		}
		if($ret[$t]==1){
			print "return ";
		}
		print "_$name( __0";
		if($check[$t]==1){
			print ",argc";
		}
		for(my $j=1;$j<=$i;$j++){
			if($passthru[$t]==1){
				print ",_$j";
			}else{
				print ",&__$j";
			}
		}
		
		print ");}))";
		
		print "\n";
	}

}
