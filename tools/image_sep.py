from PIL import Image
import configparser
import sys
import os

def spec_get_name(spec: str):
    start = max(0, spec.find("{") + 1)
    end = spec.find(":")
    if end < 0:
        end = spec.find("}")
    if end < 0:
        end = len(spec)
    return spec[start : end]

def path_spec_has_spec(pattern: str, spec: str):
    return pattern.find("{" + spec_get_name(spec))

def str_insert_at(string: str, value: str, index: int):
	if index < 0:
		index = len(string)
	return string[:index] + value + string[index:]

def path_spec_insert_spec(pattern: str, spec: str, spacer: str = "_",):
	if not path_spec_has_spec(pattern, spec):
			return str_insert_at(pattern, spacer + spec, pattern.rfind("."))
	return pattern

def remove_spec_pattern(pattern: str, spec: str):
	index = pattern.find(spec)
	if index == -1:
		return pattern
	left = ''
	if index > 0:
		left = pattern[index - 1]
	right = ''
	if index < len(pattern):
		right = pattern[index + len(spec)]
	spacer = left
	if not spacer:
		spacer = right
	if not spacer:
		spacer = '_'
	if right == '.':
		spacer = '.'
	psplit = pattern.split(spec)
	return psplit[0].rstrip(left) + spacer + psplit[1].lstrip(right)

def get_pattern(config: configparser, section: str, name: str) :
	path = config[section][name]
	if not path:
		path = config["DEFAULT"][name]
	return path

def execute_sep(cfg_path: str):
	folder = os.path.dirname(cfg_path)
	config = configparser.ConfigParser()
	config.read(cfg_path)
	for section in config.sections():
		section_u = section.upper()
		section_l = section.lower()
		path_spec: str = os.path.join(folder, get_pattern(config, section, "Img"))
		if not path_spec:
			print(f"Section: {section} has no input path spec, passing.")
			continue
		path_spec_x = path_spec_insert_spec(path_spec, "{x}")
		out_spec: str =  os.path.join(folder, get_pattern(config, section, "Out"))
		if not out_spec:
			print(f"Section: {section} has no output path spec, passing.")
			continue
		out_spec_x = path_spec_insert_spec(out_spec, "{x}")
		try:
			frames = int(config[section]["frames"])
			min_x = int(config[section]["min_x"])
			min_y = int(config[section]["min_y"])
			max_x = int(config[section]["max_x"])
			max_y = int(config[section]["max_y"])
			rect_all = f"x_{min_x}_y_{min_y}__X_{max_x}_Y_{max_y}"
			rect = f"{min_x}_{min_y}__{max_x}_{max_y}"
			box = (min_x, min_y, max_x, max_y)
			if frames >= 2:
				for x in range(1, frames + 1):
					i = x - 1
					x_hex = hex(x)
					i_hex = hex(x)
					file_full = path_spec_x.format(**vars())
					file = os.path.basename(file_full)
					im = Image.open(file_full)
					region = im.crop(box)
					print(f"saving: ({rect}) as " + out_spec_x.format(**vars()))
					if (not os.path.exists(os.path.dirname(out_spec.format(**vars())))):
						os.makedirs(os.path.dirname(out_spec.format(**vars())))
					region.save(out_spec.format(**vars()))
			else:
				im = Image.open(str.format(path_spec, **vars()))
				region = im.crop(box)
				print(f"saving: {section} as " + out_spec.format(**vars()))
				region.save(out_spec.format(**vars()))
		
		except (TypeError, ValueError) as ex:
			print(f"An error occured skipping ${section}")
			print(ex)
			pass

execute_sep(sys.argv[1])