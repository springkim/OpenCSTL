use strict;
use warnings;


my @arr=qw/ cstl_push_back cstl_push_front cstl_insert cstl_erase cstl_resize cstl_find cstl_push/;
my @check=qw/ 0 0 1 1 1 1 0/;
my @ret=qw/ 0 0 0 0 0 1 0/;
for(my $t=0;$t<=$#arr;$t++){
	my $name=$arr[$t];
	
	print "#define $name(C,...) ";

	if($ret[$t]==1){
		print "({void* _() {";
	}
	print "_linux_$name(C,__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)";
	print "(C";
	if($check[$t]==1){
		print(",ARGN(__VA_ARGS__)");
	}
	print ",__VA_ARGS__)";
	if($ret[$t]==1){
		print "}_;})();";
	}
	print "\n";
	print "#define _linux_$name(C,_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) _$name ## _ ## N","\n";
	for(my $i=0;$i<=10;$i++){
		print "#define _$name"."_$i";
		print "(C";
		if($check[$t]==1){
			print ",argc";
		}
		for(my $j=1;$j<=$i;$j++){
			print ",_$j";
		}
		
		print ")    {";
		
		print "__auto_type __0=&C;";

		for(my $j=1;$j<=$i;$j++){
			print "__auto_type __$j=_$j;";
		}
		if($ret[$t]==1){
			print "return ";
		}
		print "_$name( __0";
		if($check[$t]==1){
			print ",argc";
		}
		for(my $j=1;$j<=$i;$j++){
			print ",&__$j";
		}
		
		print ");}";
		
		print "\n";
	}

}
