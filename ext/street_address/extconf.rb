require 'mkmf'

$CFLAGS << ' -Wall -Wextra -O3 '

create_makefile('street_address/street_address_ext')