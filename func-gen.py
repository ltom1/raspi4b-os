import os
import re

SOURCE_DIR = 'src'
INCLUDE_DIR = 'include'  
H_FILE = os.path.join(INCLUDE_DIR, 'func.h')
C_FILE = os.path.join(SOURCE_DIR, 'func.c')


# Pattern to match function definitions (basic, may need adjustment)
#pattern = re.compile(r'^\s*[\w\s\*]+?\s+(\w+)\s*\([^)]*\)\s*{', re.MULTILINE)
pattern = re.compile(r'^[\w\s\*]+?\s+(\w+)\s*\([^)]*\)\s*\{', re.MULTILINE)

functions = set()

# Walk through source directory
for root, dirs, files in os.walk(SOURCE_DIR):
    for filename in files:
        if filename.endswith('.c'):
            filepath = os.path.join(root, filename)
            with open(filepath, 'r') as f:
                content = f.read()
                matches = pattern.findall(content)
                for match in matches:
                    functions.add(match)

functions.remove("while")
functions.remove("for")
functions.remove("switch")

# Generate enum and array
enum_lines = ['#pragma once\n\nextern const char* func_names[];\n\n\ntypedef enum {\n\tFUNC_NONE,']
array_lines = ['const char* func_names[] = {\n\t"",']

for i, func in enumerate(sorted(functions)):
    enum_lines.append(f'\tFUNC_{func.upper()},')
    array_lines.append(f'\t"{func}",')

enum_lines.append('\n} func_id_t;')

array_lines.append('\n};')

with open(H_FILE, 'w') as f:
    f.write('\n'.join(enum_lines))

with open(C_FILE, 'w') as f:
    f.write('\n'.join(array_lines))
