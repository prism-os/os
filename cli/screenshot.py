#!/usr/bin/python3
import serial
from PIL import Image
import time

SCREEN_WIDTH = 128
SCREEN_HEIGHT = 64
SCALE_NEEDED = False

def int_to_binary_list(int_num):
  binary_rep = '{0:0>8b}'.format(int_num)
  return list(map(int, list(binary_rep)))

def vert_fours_to_horz_raster(fours_list_list):
  ret_list = []
  for index in [7, 6, 5, 4, 3, 2, 1]:  
    for vert_list in fours_list_list:
      ret_list.append(vert_list[index])
  return ret_list

def scale_height_by_fills(raster_list):
  output_list = [raster_list[i:i+SCREEN_WIDTH] for i in range(0, len(raster_list), SCREEN_WIDTH)]
  ret_list = []
  for index in range(len(output_list)):
    ret_list.append(output_list[index])
    ret_list.append([0]*SCREEN_WIDTH)
  ret_list = [item for sublist in ret_list for item in sublist]
  return ret_list

def monochrome_to_rgba(pixel):
  if(pixel==0):
    return (0, 0, 0, 0)
  else:
    return (255, 255, 255, 255)

def perform(port, baudrate, output_file_path):
  ser = serial.Serial(port=port, baudrate=baudrate, timeout=5)
  command = 's'
  expected_end_part = '}\r\n'
  expected_start_part = command + ':{'
  print('Sending request to attached device...')
  ser.write(command.encode())
  time.sleep(2)
  read_chars = []
  while ser.in_waiting:
    read_chars.append(ser.read())
  if len(read_chars) > 4:
    received_start_part = "".join(map(lambda bytechar : bytechar.decode(), read_chars[0:3]))
    received_end_part = "".join(map(lambda bytechar : bytechar.decode(), read_chars[len(read_chars) - 3:len(read_chars)]))
    if received_start_part == expected_start_part and received_end_part == expected_end_part:
      read_hex_chars = read_chars[len(expected_start_part):len(read_chars) - len(expected_end_part)]
      binary_read_chars = list(map(lambda bytechar : bytechar[0], read_hex_chars))
      framebuf_bin_listX2 = list(map(int_to_binary_list, binary_read_chars))
      framebuf_bin_listX3 = [framebuf_bin_listX2[i:i+SCREEN_WIDTH] for i in range(0, len(framebuf_bin_listX2), SCREEN_WIDTH)]
      final_raster_list = [item for sublist in list(map(vert_fours_to_horz_raster, framebuf_bin_listX3)) for item in sublist]
      final_rgba_raster_list = list(map(monochrome_to_rgba, final_raster_list))
      i = Image.new("RGBA", (SCREEN_WIDTH,SCREEN_HEIGHT), (0, 0, 0, 0))
      if SCALE_NEEDED:
        scaled_up_raster_list = scale_height_by_fills(final_rgba_raster_list)
        i.putdata(scaled_up_raster_list)
      else:
        i.putdata(final_rgba_raster_list)
      i.save(output_file_path, "PNG")
      print('Image saved')
      return 0
    else:
      print('Invalid response, make sure you are running a debug firmware and their are no Serial prints in the app you are taking a screenshot of')
      return 1
  else:
    print('Invalid response, make sure you are running a debug firmware and their are no Serial prints in the app you are taking a screenshot of')
    return 1

