% The .text contents is supposed to be linked --oformat binary with
% b-twoinsn.s, and will provide a valid end of a mmo file, but with the
% "n" in :Main in wide character format; the widening is with a 0xff, so it
% isn't supported, and the file is rejected.
 .text
 .byte 0x98,0x0b,0x00,0x00,0x20,0x3a,0x30,0x4d,0x20,0x61,0x20,0x69
 .byte 0x81,0xff,0x6e,0x04,0x81,0x01,0x61,0x04,0x82,0,0,0,0x98,0x0c,0x00,0x05
