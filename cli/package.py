#!/usr/bin/python3
import os
import json
import tempfile
import shutil
import textwrap
import re
import zipfile
from subprocess import call

AUTOGEN_FILES = ['version.cpp', 'insert_app_caller.cpp']

def generate_autogen_version(account_id, image_id, source_dir):
  template = textwrap.dedent(
    '''extern const char USER_ACCOUNT_ID[{len_account_id}] = "{account_id}";
    extern const char USER_IMAGE_ID[{len_image_id}] = "{image_id}";
    extern const char SYSTEM_IMAGE_ID[{len_version}] = "{version}";'''
  )
  with open(os.path.join(source_dir, 'version.txt'), 'rt') as fin:
    version = fin.readline()[:-1]
    content = template.format(account_id=account_id, image_id=image_id, version=version, len_account_id=str(len(account_id) + 1), len_image_id=str(len(image_id) + 1), len_version=str(len(version) + 1))
    return content

def generate_autogen_insert_app_caller(entries):
  template = textwrap.dedent(
    '''#ifndef APPS_H
	    #include <prism/apps.hpp>
    #endif
    #ifndef APP_WRAPPER_CLASS_H
      #include <prism/app_wrapper.hpp>
    #endif'''
  )
  template += '\n'
  template += '\n'
  for entry in entries:
    template += '#include "{header_file_include_path}"'.format(header_file_include_path=entry['header_file_include_path'])
    template += '\n'
  template += '\n'
  for entry in entries:
    template += '{class_name} {class_name}_object;'.format(class_name=entry['class_name'])
    template += '\n'
  template += '\n'
  for entry in entries:	
    template += 'AppWrapper {class_name}_object_wrapper("{name}", {widget_ability}, {notification_ability}, &{class_name}_object);'.format(class_name=entry['class_name'], name=entry['name'] if len(entry['name']) < 14 else entry['name'][:11] + '..', widget_ability='true' if entry['abilities']['widget'] else 'false', notification_ability='true' if entry['abilities']['notification'] else 'false')	
    template += '\n'	
  template += '\n'
  template += 'void AppsClass::_insertAppCaller(void) {'
  template += '\n'
  for entry in entries:
    template += '_insertApp({id}, &{class_name}_object_wrapper);'.format(id=entry['id'], class_name=entry['class_name'])
    template += '\n'
  template += '}'
  template += '\n'
  return template

def generate_autogen_files(build_dir, account_id, image_id, entries, source_dir):
  for file in AUTOGEN_FILES:
    with open(os.path.join(build_dir, file), 'wt') as fout:
      if (file == 'version.cpp'):
        fout.write(generate_autogen_version(account_id, image_id, source_dir))
      elif (file == 'insert_app_caller.cpp'):
        fout.write(generate_autogen_insert_app_caller(entries))

def list_all_header_files(path):
  onlyfiles = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and f.split('.')[-1] == 'hpp']
  return onlyfiles

def list_all_app_source_directory(path):
  onlydirs = [f for f in os.listdir(path) if os.path.isdir(os.path.join(path, f)) and f.isdigit()]
  return onlydirs

def list_all_non_header_non_autgen_source_files(path):
  onlyfiles = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and f.split('.')[-1] == 'cpp' and (f not in AUTOGEN_FILES)]
  return onlyfiles

def list_all_apkg_file_paths(path):
  onlyfiles = [os.path.join(path, f) for f in os.listdir(path) if os.path.isfile(os.path.join(path, f)) and f.split('.')[0].isdigit() and f.split('.')[-1] == 'apkg']
  return onlyfiles

def copy_all_non_header_non_autogen_source_files(build_dir, source_dir):
  for file in list_all_non_header_non_autgen_source_files(os.path.join(source_dir, 'framework', 'prism')):
    shutil.copy(os.path.join(source_dir, 'framework', 'prism', file), build_dir)

def copy_source_archive_file(build_dir, source_dir, debug):
  if debug:
    shutil.copy(os.path.join(source_dir, 'framework', 'main.debug.a'), build_dir)
  else:
    shutil.copy(os.path.join(source_dir, 'framework', 'main.a'), build_dir)

def copy_main_ino_file(build_dir, source_dir):
  shutil.copy(os.path.join(source_dir, 'framework', 'prism', 'entry.ino'), build_dir)

def unzip_apkg_into_directory(apkg_file_path, app_source_directory):
  zip_ref = zipfile.ZipFile(apkg_file_path, 'r')
  zip_ref.extractall(app_source_directory)
  zip_ref.close()

def process_and_copy_apps(build_dir, source_dir, apkgs_dir_or_apkg_file, debug):
  # for processing if apps_dir is not supplied, we copy over files from framework that are having only number file names since those represent app files
  apps_dir = tempfile.mkdtemp()
  apkg_file_paths = []
  if apkgs_dir_or_apkg_file == None:
    apkg_file_paths = []
  elif os.path.isdir(apkgs_dir_or_apkg_file):
    apkg_file_paths.extend(list_all_apkg_file_paths(apkgs_dir_or_apkg_file))
  elif os.path.isfile(apkgs_dir_or_apkg_file) and os.path.basename(os.path.normpath(apkgs_dir_or_apkg_file)).split('.')[-1] == 'apkg':
    apkg_file_paths.append(apkgs_dir_or_apkg_file)
  for apkg_file_path in apkg_file_paths:
    series_id = os.path.basename(os.path.normpath(apkg_file_path)).split('.')[0]
    os.mkdir(os.path.join(apps_dir, series_id))
    unzip_apkg_into_directory(apkg_file_path, os.path.join(apps_dir, series_id))
  app_dirs = list_all_app_source_directory(apps_dir)
  entries = []
  for series_id in app_dirs:
    with open(os.path.join(apps_dir, series_id, 'manifest.json'), 'rt') as fin:
      manifest_string = fin.read()
      parsed_manifest = json.loads(manifest_string)
      main_header_file_include_path = './{series_id}/main.hpp'.format(series_id=series_id)
      ar_file_name = '{series_id}.a'.format(series_id=series_id)
      entries.append({**parsed_manifest, 'header_file_include_path': main_header_file_include_path, 'id': series_id})
      shutil.copy(os.path.join(apps_dir, series_id, 'main.debug.a' if debug else 'main.a'), os.path.join(build_dir, ar_file_name))
      os.mkdir(os.path.join(build_dir, series_id))
      header_files_list = list_all_header_files(os.path.join(apps_dir, series_id))
      for header_file in header_files_list:
        shutil.copy(os.path.join(apps_dir, series_id, header_file), os.path.join(build_dir, series_id, header_file))
  shutil.rmtree(apps_dir)
  return entries

def perform(apps_directory_or_app_file_path, debug, account_id, image_id, source_dir, output_path, flash, port, baudrate):
  app_entries = []
  build_dir = tempfile.mkdtemp()
  # copy_all_non_header_non_autogen_source_files(build_dir, source_dir)
  copy_source_archive_file(build_dir, source_dir, debug)
  copy_main_ino_file(build_dir, source_dir)
  app_entries.extend(process_and_copy_apps(build_dir, source_dir, apps_directory_or_app_file_path, debug))
  generate_autogen_files(build_dir, account_id, image_id, app_entries, source_dir)
  build_command = ['make', '-f', 'mainMake.mk', 'SKETCH={path}'.format(path=os.path.join(build_dir, 'entry.ino')), 'BUILD_ROOT={path}'.format(path=build_dir)]
  if port != 'NULL':
    build_command.append('UPLOAD_PORT={port}'.format(port=port))
  if baudrate != 'NULL':
    build_command.append('UPLOAD_SPEED={baudrate}'.format(baudrate=baudrate))
  if flash:
    build_command.append('upload')
  if debug:
    build_command.append('DEBUG=1')
  current_dir = os.getcwd()
  os.chdir(source_dir)
  returncode = call(build_command)
  os.chdir(current_dir)
  if (returncode == 0):
    shutil.copy(os.path.join(build_dir, 'entry_nodemcuv2', 'entry.bin'), output_path)
    shutil.rmtree(build_dir)
    # if (debug):
    #   print('Copying elf for parsing stack traces...')
    #   print(build_dir)
    #   shutil.copy(os.path.join(build_dir, 'entry_nodemcuv2', 'entry.elf'), output_path[:-5] + 'elf')
    #   print(os.path.join(build_dir, 'entry_nodemcuv2', 'entry.elf'))
    #   print(output_path[:-5] + 'elf')
    #   # shutil.rmtree(build_dir)
  else:
    print('Temp build location:')
    print(build_dir)
  return returncode
