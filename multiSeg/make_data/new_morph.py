import json
from tqdm import tqdm

seg_morph_data = "../segmentations/morph_data/"

morph_maps = {
'list_SW_TL' : [
	('../data/IARPA/SW/iarpa_sw.en', '../segmentations/morph_data/english-bitext-morph_sw.txt', '../segmentations/morph_map/sw_morph_3p_map.en', '../segmentations/morph_map/sw_morph_3_map.en'), 
	('../data/IARPA/SW/iarpa_sw.sw', '../segmentations/morph_data/swahili-bitext-morph_sw.txt', '../segmentations/morph_map/sw_morph_3p_map.sw', '../segmentations/morph_map/sw_morph_3_map.sw'),
	('../data/IARPA/TL/iarpa_tl.en', '../segmentations/morph_data/english-bitext-morph_tl.txt', '../segmentations/morph_map/tl_morph_3p_map.en', '../segmentations/morph_map/tl_morph_3_map.en'), 
	('../data/IARPA/TL/iarpa_tl.tl', '../segmentations/morph_data/tagalog-bitext-morph_tl.txt', '../segmentations/morph_map/tl_morph_3p_map.tl', '../segmentations/morph_map/tl_morph_3_map.tl')],
'list_SW_TL_1_' : [
	('../data/IARPA/SW/iarpa_sw.en', '../segmentations/morph_data/english-bitext-morph_sw.txt', '../segmentations/morph_map/sw_morph_3p_map.en', '../segmentations/morph_map/sw_morph_3_map.en')],
'list_morph_DE_12345' : [
	('../data/Europarl/mini/1k_europarl.cldc.de', '../segmentations/morph_data/Europarl/mini/1k_europarl.cldc.de-seg.txt', '../segmentations/morph_map/Europarl/mini/1k_europarl.cldc.de-seg-3p_morph_map.de'), 
	('../data/Europarl/mini/1k_europarl.cldc.en', '../segmentations/morph_data/Europarl/mini/1k_europarl.cldc.en-seg.txt', '../segmentations/morph_map/Europarl/mini/1k_europarl.cldc.en-seg-3p_morph_map.en'),
	('../data/Europarl/mini/1k_europarl.cldc.de', '../segmentations/morph_data/Europarl/mini/1k_europarl.cldc.de-seg-3.txt', '../segmentations/morph_map/Europarl/mini/1k_europarl.cldc.de-seg-3_morph_map.de'), 
	('../data/Europarl/mini/1k_europarl.cldc.en', '../segmentations/morph_data/Europarl/mini/1k_europarl.cldc.en-seg-3.txt', '../segmentations/morph_map/Europarl/mini/1k_europarl.cldc.en-seg-3_morph_map.en'),
	
	('../data/Europarl/mini/10k_europarl.cldc.de', '../segmentations/morph_data/Europarl/mini/10k_europarl.cldc.de-seg.txt', '../segmentations/morph_map/Europarl/mini/10k_europarl.cldc.de-seg-3p_morph_map.de'), 
	('../data/Europarl/mini/10k_europarl.cldc.en', '../segmentations/morph_data/Europarl/mini/10k_europarl.cldc.en-seg.txt', '../segmentations/morph_map/Europarl/mini/10k_europarl.cldc.en-seg-3p_morph_map.en'),
	('../data/Europarl/mini/10k_europarl.cldc.de', '../segmentations/morph_data/Europarl/mini/10k_europarl.cldc.de-seg-3.txt', '../segmentations/morph_map/Europarl/mini/10k_europarl.cldc.de-seg-3_morph_map.de'), 
	('../data/Europarl/mini/10k_europarl.cldc.en', '../segmentations/morph_data/Europarl/mini/10k_europarl.cldc.en-seg-3.txt', '../segmentations/morph_map/Europarl/mini/10k_europarl.cldc.en-seg-3_morph_map.en'),
	
	('../data/Europarl/mini/100k_europarl.cldc.de', '../segmentations/morph_data/Europarl/mini/100k_europarl.cldc.de-seg.txt', '../segmentations/morph_map/Europarl/mini/100k_europarl.cldc.de-seg-3p_morph_map.de'), 
	('../data/Europarl/mini/100k_europarl.cldc.en', '../segmentations/morph_data/Europarl/mini/100k_europarl.cldc.en-seg.txt', '../segmentations/morph_map/Europarl/mini/100k_europarl.cldc.en-seg-3p_morph_map.en'),
	('../data/Europarl/mini/100k_europarl.cldc.de', '../segmentations/morph_data/Europarl/mini/100k_europarl.cldc.de-seg-3.txt', '../segmentations/morph_map/Europarl/mini/100k_europarl.cldc.de-seg-3_morph_map.de'), 
	('../data/Europarl/mini/100k_europarl.cldc.en', '../segmentations/morph_data/Europarl/mini/100k_europarl.cldc.en-seg-3.txt', '../segmentations/morph_map/Europarl/mini/100k_europarl.cldc.en-seg-3_morph_map.en'),

	('../data/Europarl/europarl.cldc.de', '../segmentations/morph_data/Europarl/europarl.cldc.de-seg.txt', '../segmentations/morph_map/Europarl/europarl.cldc.de-seg-3p_morph_map.de'), 
	('../data/Europarl/europarl.cldc.en', '../segmentations/morph_data/Europarl/europarl.cldc.en-seg.txt', '../segmentations/morph_map/Europarl/europarl.cldc.en-seg-3p_morph_map.en'),
	('../data/Europarl/europarl.cldc.de', '../segmentations/morph_data/Europarl/europarl.cldc.de-seg-3.txt', '../segmentations/morph_map/Europarl/europarl.cldc.de-seg-3_morph_map.de'), 
	('../data/Europarl/europarl.cldc.en', '../segmentations/morph_data/Europarl/europarl.cldc.en-seg-3.txt', '../segmentations/morph_map/Europarl/europarl.cldc.en-seg-3_morph_map.en')],
'list_morph_DE_2_' : [
	('../data/Europarl/mini/1k_europarl.cldc.de', '../segmentations/morph_data/Europarl/mini/1k_europarl.cldc.de-seg.txt', '../segmentations/morph_map/Europarl/mini/1k_europarl.cldc.de-seg-3p_morph_map.de')	
	]
}

for list_morph in morph_maps:
	if(list_morph == 'list_morph_DE'):
		for file_src, file_in, file_out in morph_maps[list_morph]:
			print("{}\t{}\t{}\t{}".format(list_morph, file_src, file_in, file_out))
			f1_src = open(file_src, "r")

			f_o_word_recreate = open(file_out + ".word_recreate.txt", 'w')
			f_o_stem_recreate = open(file_out + ".stem_recreate.txt", 'w')

			word_map = {}
			for line in tqdm(open(file_in)):
				new_recreate_word_line = ""
				new_recreate_stem_line = ""

				line_src = f1_src.readline()
				#line, line_src = line.strip(), line_src.strip()
				line_split, line_src_split = line.split(), line_src.split()
				assert(len(line_split) == len(line_src_split))

				for word_id in range(len(line_src_split)):
					word_map[line_src_split[word_id]] = [line_src_split[word_id]]

					if(line_split[word_id].startswith('(')) and line_split[word_id].endswith(')'):
						word_map[line_src_split[word_id]].append(line_split[word_id][1:-1])
						new_recreate_stem_line = new_recreate_stem_line + line_split[word_id][1:-1]
						new_recreate_word_line = new_recreate_word_line + line_split[word_id][1:-1]
					else:
						morph_split = line_split[word_id].split('+')
						for morphing in morph_split:
							if(morphing.startswith('(')) and morphing.endswith(')'):
								word_map[line_src_split[word_id]].append(morphing[1:-1])
								new_recreate_word_line = new_recreate_word_line + morphing[1:-1]

								if(morphing == "()"):
									morphing = "(<empty>)"
								new_recreate_stem_line = new_recreate_stem_line + morphing[1:-1]
								
							else:
								word_map[line_src_split[word_id]].append(morphing)
								new_recreate_word_line = new_recreate_word_line + morphing

					new_recreate_stem_line = new_recreate_stem_line + ' '
					new_recreate_word_line = new_recreate_word_line + ' '
				
				f_o_stem_recreate.write(new_recreate_stem_line[:-1] + "\n")
				f_o_word_recreate.write(new_recreate_word_line[:-1] + "\n")
						
			with open(file_out, 'w') as f_o:
				for key, value in word_map.items():
					for word_morph_parts in value:
						f_o.write(word_morph_parts + "\t")
					f_o.write('\n')
			f_o_stem_recreate.close()
			f_o_word_recreate.close()

	elif(list_morph == 'list_SW_TL'):
		for file_src, file_in, file_out_3p, file_out_3 in morph_maps[list_morph]:
			print("{}\t{}\t{}\t{}".format(list_morph, file_in, file_out_3p, file_out_3))
			word_map = {}
			word_map_3 = {}
			f1_src = open(file_src, "r")
			f_o_word_recreate = open(file_out_3p + ".word_recreate.txt", "w")
			f_o_stem_recreate = open(file_out_3p + ".stem_recreate.txt", "w")

			for line in open(file_in):
				a = json.loads(line)
				new_recreate_stem_line = ""
				new_recreate_word_line = ""

				for sentence_id in range(len(a)):
					for word_id in range(len(a[sentence_id])):
						prefix = ""
						stem = ""
						postfix = ""
						word = a[sentence_id][word_id]["word"]
						word = word.replace("-LRB-", "(").replace("-RRB-", ")")
						word = '\'' if word == '`' else word

						word_map[word] = [word]
						word_map_3[word] = [word]
						if(a[sentence_id][word_id]["prefixes"]):
							a[sentence_id][word_id]["prefixes"] = a[sentence_id][word_id]["prefixes"].replace("-LRB-", "(").replace("-RRB-", ")")
							for word_abc in a[sentence_id][word_id]["prefixes"].split('+'):
								word_map[word].append(word_abc)
								prefix = prefix + word_abc
							word_map_3[word].append(prefix)

						if(a[sentence_id][word_id]["stem"]):
							stem = a[sentence_id][word_id]["stem"].replace("-LRB-", "(").replace("-RRB-", ")")
							#stem = '\'' if stem == '`' else stem
							word_map[word].append(stem)
							word_map_3[word].append(stem)

							if(stem == ""):
								stem = "<EMPTY>"
						if(a[sentence_id][word_id]["suffixes"]):
							a[sentence_id][word_id]["suffixes"] = a[sentence_id][word_id]["suffixes"].replace("-LRB-", "(").replace("-RRB-", ")")
							for word_abc in a[sentence_id][word_id]["suffixes"].split('+'):
								word_map[word].append(word_abc)
								postfix = postfix + word_abc
							word_map_3[word].append(postfix)
					
						new_recreate_stem_line = new_recreate_stem_line + stem + ' '
						#new_recreate_word_line = new_recreate_word_line + prefix + stem + postfix + ' '
						new_recreate_word_line = new_recreate_word_line + word + ' '

				# new_recreate_word_line = new_recreate_word_line.replace('`` ', '\'\'')
				# new_recreate_stem_line = new_recreate_stem_line.replace('`` ', '\'\'')

				# new_recreate_word_line = new_recreate_word_line.replace(' ` ', ' \' ')
				# new_recreate_stem_line = new_recreate_stem_line.replace(' ` ', ' \' ')
				f_o_stem_recreate.write(new_recreate_stem_line[:-1] + "\n")
				f_o_word_recreate.write(new_recreate_word_line[:-1] + "\n")

			with open(file_out_3p, 'w') as f_o:
				for key, value in word_map.items():
					for word_morph_parts in value:
						f_o.write(word_morph_parts + "\t")
					f_o.write('\n')

			with open(file_out_3, 'w') as f_o:
				for key, value in word_map_3.items():
					for word_morph_parts in value:
						f_o.write(word_morph_parts + "\t")
					f_o.write('\n')

			f_o_stem_recreate.close()
			f_o_word_recreate.close()

