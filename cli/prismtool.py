#!/usr/bin/python3
import os
import re
import shutil
import argparse
import datetime
import sys
import tempfile
import argparse_prompt

import screenshot
import package
import build

INITIAL_ACCOUNT_ID = 'initial'
INITIAL_IMAGE_ID = 'initial'
DEBUG_ACCOUNT_ID = 'debug'
DEBUG_IMAGE_ID = 'debug'

image_file_name = 'prism_{date:%Y-%m-%d_%H-%M-%S}.image'.format(date=datetime.datetime.now())
screenshot_file_name = 'screenshot_{date:%Y-%m-%d_%H-%M-%S}.png'.format(date=datetime.datetime.now())

PACKAGE_DIR = ''
IS_RUNNING_FROM_COMPILED_BINARY = False
if getattr( sys, 'frozen', False ):
  IS_RUNNING_FROM_COMPILED_BINARY = True
  PACKAGE_DIR = os.path.abspath(os.path.join(os.path.dirname(sys.executable), os.path.pardir))
else:
  PACKAGE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir))

def package_subcommand(args):
  returncode = package.perform(args.apps_directory_or_app_file_path, args.debug, args.account_id, args.image_id, PACKAGE_DIR, args.output_path, args.flash, args.port, args.baudrate)
  sys.exit(0 if returncode == 0 else 1)

def screenshot_subcommand(args):
  returncode = screenshot.perform(args.port, args.baudrate if not IS_RUNNING_FROM_COMPILED_BINARY else 9600, args.output_path)
  sys.exit(0 if returncode == 0 else 1)

def build_subcommand(args):
  returncode = build.perform(args.src_path, args.out_path, PACKAGE_DIR, args.debug if not IS_RUNNING_FROM_COMPILED_BINARY else False)
  sys.exit(0 if returncode == 0 else 1)

def run_subcommand(args):
  build_dir = tempfile.mkdtemp()
  returncode = build.perform(args.src_path, build_dir, PACKAGE_DIR, not args.non_debug if not IS_RUNNING_FROM_COMPILED_BINARY else True)
  if returncode == 0:
    os.rename(os.path.join(build_dir, 'main.apkg'), os.path.join(build_dir, '7.apkg'))
    returncode = package.perform(os.path.join(build_dir, '7.apkg'), not args.non_debug if not IS_RUNNING_FROM_COMPILED_BINARY else True, DEBUG_ACCOUNT_ID, DEBUG_IMAGE_ID, PACKAGE_DIR, os.path.join(build_dir, image_file_name), True, args.port, args.baudrate)
  if (returncode == 0):
    shutil.rmtree(build_dir)
    sys.exit(0)
  else:
    sys.exit(1)

def install_subcommand(args):
  build_dir = tempfile.mkdtemp()
  returncode = package.perform(build_dir, False, INITIAL_ACCOUNT_ID, INITIAL_IMAGE_ID, PACKAGE_DIR, os.path.join(build_dir, image_file_name), True, args.port, args.baudrate)
  if (returncode == 0):
    shutil.rmtree(build_dir)
    sys.exit(0)
  else:
    sys.exit(1)

def validate_class_name_parts(string, min_len, max_len):
  pattern = re.compile("^([a-zA-Z][a-zA-Z0-9_]*)+$")
  if not isinstance(string, str):
    return False
  if len(string) < min_len or len(string) > max_len:
    return False
  if not pattern.match(string):
    return False
  if '__' in string:
    return False
  return True

def validate_name(string, min_len, max_len):
  pattern = re.compile("^([a-zA-Z0-9 ]+)+$")
  if not isinstance(string, str):
    return False
  if len(string) < min_len or len(string) > max_len:
    return False
  if not pattern.match(string):
    return False
  return True

def new_subcommand(args):
  if not validate_class_name_parts(args.developer_name, 5, 20):
    print('Enter a valid developer name registered on the Prism Store')
    return
  if not validate_name(args.name, 1, 13):
    print('Enter an app name containing only a-z, A-Z, 0-9 and spaces only, within 14 characters')
    return
  if not validate_class_name_parts(args.class_name_app_section, 1, 20):
    print('Enter a valid class name app section, must contain a-z, A-Z, 0-9 and underscore only, should start with only a-z or A-Z, not contain double underscores and should be 1 to 20 characters')
    return
  # If user has selected both widget and notification ability,
  # or if they have selected neither, then in default case,
  # create app with both abilities
  template_dir = 'both_notification_widget_ability'
  if args.widget != args.notification:
    if args.widget:
      template_dir = 'only_widget_ability'
    if args.notification:
      template_dir = 'only_notification_ability'
  os.mkdir(args.path)
  for template_source_file in ['main.hpp', 'main.cpp', 'manifest.json']:
    with open(os.path.join(PACKAGE_DIR, 'apps', 'templates', template_dir, template_source_file), 'rt') as fin:
      data = fin.read().replace('@@NAME@@', args.name).replace('@@USERNAME@@', args.developer_name).replace('@@CLASSNAME@@', args.class_name_app_section)
      with open(os.path.join(args.path, template_source_file), 'wt') as fout:
        fout.write(data)

parser = argparse_prompt.PromptParser(description='Create, build, run apps for Prism OS')
subparsers = parser.add_subparsers()

run_parser = subparsers.add_parser('run', help='Run app on connected Prism OS compatible board')
run_parser.add_argument('--src-path', '-s', default=os.getcwd(), help='Path to source directory, defaults to current directory', prompt=False)
run_parser.add_argument('--port', '-p', default='NULL', help='Serial port where board is connected', prompt=False)
run_parser.add_argument('--baudrate', '-b', default='NULL', help='Baud rate for communicating with serial port for flashing', prompt=False)
if not IS_RUNNING_FROM_COMPILED_BINARY:
  run_parser.add_argument('--non-debug', '-n', action='store_true', help='Run non debug version of the app', prompt=False)
run_parser.set_defaults(func=run_subcommand)

if not IS_RUNNING_FROM_COMPILED_BINARY:
  package_parser = subparsers.add_parser('package', help='Build and flash images')
  package_parser.add_argument('--flash', '-f', action='store_true', help='Flash right after building onto port defined in mainMake.mk', prompt=False)
  package_parser.add_argument('--debug', '-d', action='store_true', help='Build developer version of the image', prompt=False)
  package_parser.add_argument('--output-path', '-o', default=os.path.join(PACKAGE_DIR, 'outputs', image_file_name), help='Path to output the image file, builds to outputs folder by default', prompt=False)
  package_parser.add_argument('--account-id', '-a', default=INITIAL_ACCOUNT_ID, help='Id of the account that the build has to be associated with, set to \'initial\' by default and would need ota flashing from mobile app after first connection', prompt=False)
  package_parser.add_argument('--image-id', '-i', default=INITIAL_IMAGE_ID, help='Unique id associated with the build, set by the build service and usually should not be set from cli, set to \'initial\' by default and would need ota flashing from mobile app after first connection', prompt=False)
  package_parser.add_argument('--apps-directory-or-app-file-path', '-s', help='Path to directory containing app files or path to a single app file, leave empty to create stock image', prompt=False)
  package_parser.add_argument('--port', '-p', default='NULL', help='Serial port where board is connected', prompt=False)
  package_parser.add_argument('--baudrate', '-b', default='NULL', help='Baud rate for communicating with serial port for flashing', prompt=False)
  package_parser.set_defaults(func=package_subcommand)

build_parser = subparsers.add_parser('build', help='Build app package for publishing')
build_parser.add_argument('--src-path', '-s', default=os.getcwd(), help='Path to source directory, defaults to current directory', prompt=False)
build_parser.add_argument('--out-path', '-o', default=os.getcwd(), help='Path to output package directory, defaults to current directory', prompt=False)
if not IS_RUNNING_FROM_COMPILED_BINARY:
  build_parser.add_argument('--debug', '-d', action='store_true', help='Compile debug version', prompt=False)
build_parser.set_defaults(func=build_subcommand)

install_parser = subparsers.add_parser('install', help='Install Prism OS on connected board')
install_parser.add_argument('--port', '-p', default='NULL', help='Serial port where board is connected', prompt=False)
install_parser.add_argument('--baudrate', '-b', default='NULL', help='Baud rate for communicating with serial port for flashing', prompt=False)
install_parser.set_defaults(func=install_subcommand)

new_parser = subparsers.add_parser('new', help='Generate new Prism OS application')
new_parser.add_argument('path', help='Path to source directory', prompt=False)
new_parser.add_argument('--widget', action='store_true', help='Enable application to show up as widget', prompt=False)
new_parser.add_argument('--notification', action='store_true', help='Enable application to send notifications', prompt=False)
new_parser.add_argument('--developer-name', '-d', help='The unique developer name registered on the Prism Store')
new_parser.add_argument('--name', '-n', default='My new app', help='Application name to be displayed, must be less than 14 characters')
new_parser.add_argument('--class-name-app-section', '-c', default='my_new_app', help='Underscore separated valid variable name, to be used as part of the application\'s unique classname')
new_parser.set_defaults(func=new_subcommand)

screenshot_parser = subparsers.add_parser('screenshot', help='Take screenshot of connected board over serial')
screenshot_parser.add_argument('--port', '-p', default='/dev/ttyUSB0', help='Serial port where board is connected', prompt=False)
if not IS_RUNNING_FROM_COMPILED_BINARY:
  screenshot_parser.add_argument('--baudrate', '-b', default=9600, help='Baud rate for communicating with serial port', prompt=False)
screenshot_parser.add_argument('--output-path', '-o', default=os.path.join(os.getcwd(), screenshot_file_name), help='Path to output the screenshot file, saves to current directory by default', prompt=False)
screenshot_parser.set_defaults(func=screenshot_subcommand)

if len(sys.argv) == 1:
  args = parser.parse_args(['-h'])
else:
  args = parser.parse_args()
  args.func(args)
