import os
import tempfile
import shutil
from subprocess import call

def copy_source_files(build_dir, src_dir):
  src_files = os.listdir(src_dir)
  for source_file in src_files:
    shutil.copy(os.path.join(src_dir, source_file), build_dir)

def perform(src_dir, out_dir_path, source_dir, debug):
  #PACKAGE_SRC_MAIN_FILE_PATH PACKAGE_OUT_FILE_PATH
  build_dir = tempfile.mkdtemp()
  # copy_framework_files(build_dir, source_dir)
  copy_source_files(build_dir, src_dir)
  build_command = ['make', '-f', 'mainMake.mk', 'compile', 'DEMO=1', 'PACKAGE_SRC_DIR_PATH={path}'.format(path=build_dir), 'PACKAGE_OUT_DIR_PATH={path}'.format(path=out_dir_path)]
  if (debug):
    build_command.append('DEBUG=1')
  current_dir = os.getcwd()
  os.chdir(source_dir)
  returncode = call(build_command)
  os.chdir(current_dir)
  shutil.rmtree(build_dir)
  return returncode
