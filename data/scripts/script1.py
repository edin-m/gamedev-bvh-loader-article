#import hello_ext

#hello_ext.greet()

import CppMod

#cpp = CppMod()
print cpp.get_num()

def call_me():
	print "wicked"

import cairo

_cdata = bytearray(4*1000*1000)

def cairo_draw():

	s = cairo.ImageSurface(cairo.FORMAT_ARGB32, 1000, 1000)
	
#	import pystacia			
	#image = pystacia.read_raw(bytes(_cdata), 'rgb', 1000, 1000, 8)
	
	surface = s.create_for_data(_cdata, cairo.FORMAT_ARGB32, 1000, 1000)
	cr = cairo.Context(surface)

	cr.set_source_rgba(0, 1, 0, 0.9)
	cr.set_line_width(3.5)
	
	cr.move_to(50, 50)
	cr.rel_line_to(100, 0)
	cr.rel_line_to(0, 100)
	cr.rel_line_to(-100, 0)
	cr.rel_line_to(0, -100)
	
	cr.move_to(850, 50)
	cr.rel_line_to(100, 0)
	cr.rel_line_to(0, 100)
	cr.rel_line_to(-100, 0)
	cr.rel_line_to(0, -100)
	
	cr.move_to(850, 850)
	cr.rel_line_to(100, 0)
	cr.rel_line_to(0, 100)
	cr.rel_line_to(-100, 0)
	cr.rel_line_to(0, -100)
	
	cr.fill()

#	import pystacia		
#	img = pystacia.read_raw(bytes(_cdata), 'rgb', 1000, 1000, 8)
#	img.blur(45)
	
#	_cdata2 = _cdata
	
#	_cdata2 = bytearray(img.get_blob('rgb'))
	
	surface.write_to_png("/home/edin/dafaka2.png")
	
	c = str(_cdata)
	cdata.set_data(c)
	
	



