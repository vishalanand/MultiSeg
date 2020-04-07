import json
from tqdm import tqdm
import re, string

def normalize(s):
    #return re.sub('[%s]' % chars, '0', s).lower()
    return re.sub("\d", "0", s).lower().replace("-LRB-", "(").replace("-RRB-", ")")


seg_morph_data = "../segmentations/morph_data/"

morph_maps = {
'list_SW_TL_new' : [
	('../../data/IARPA/SW/swahili-bitext-morph_sw.txt', '../segmentations/morph_SW_TL/sw.word.sw', '../segmentations/morph_SW_TL/sw.stem.sw', '../segmentations/morph_SW_TL/sw_morph_3p_map.sw', '../segmentations/morph_SW_TL/sw_morph_3_map.sw'),
	('../../data/IARPA/SW/english-bitext-morph_sw.txt', '../segmentations/morph_SW_TL/sw.word.en', '../segmentations/morph_SW_TL/sw.stem.en', '../segmentations/morph_SW_TL/sw_morph_3p_map.en', '../segmentations/morph_SW_TL/sw_morph_3_map.en'),
	('../../data/IARPA/TL/tagalog-bitext-morph_tl.txt', '../segmentations/morph_SW_TL/tl.word.tl', '../segmentations/morph_SW_TL/tl.stem.tl', '../segmentations/morph_SW_TL/tl_morph_3p_map.tl', '../segmentations/morph_SW_TL/tl_morph_3_map.tl'),
	('../../data/IARPA/TL/english-bitext-morph_tl.txt', '../segmentations/morph_SW_TL/tl.word.en', '../segmentations/morph_SW_TL/tl.stem.en', '../segmentations/morph_SW_TL/tl_morph_3p_map.en', '../segmentations/morph_SW_TL/tl_morph_3_map.en')]	
}

for list_morph in morph_maps:
	if(list_morph == 'list_SW_TL_new'):
		#for file_src, file_in, file_out_3p, file_out_3 in morph_maps[list_morph]:
		for file_in, file_word, file_stem, file_out_3p, file_out_3 in morph_maps[list_morph]:
			print("{}:\t{}\t{}\t{}\t{}\t{}".format(list_morph, file_in, file_word, file_stem, file_out_3p, file_out_3))
			word_map = {}
			word_map_3 = {}
			#f1_src = open(file_src, "r")
			f_o_word_recreate = open(file_word, "w")
			f_o_stem_recreate = open(file_stem, "w")

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
						word = normalize(word)
						#word = '\'' if word == '`' else word

						word_map[word] = [word]
						word_map_3[word] = [word]
						if(a[sentence_id][word_id]["prefixes"]):
							a[sentence_id][word_id]["prefixes"] = normalize(a[sentence_id][word_id]["prefixes"])
							for word_abc in a[sentence_id][word_id]["prefixes"].split('+'):
								word_map[word].append(word_abc)
								prefix = prefix + word_abc
							word_map_3[word].append(prefix)

						if(a[sentence_id][word_id]["stem"]):
							stem = normalize(a[sentence_id][word_id]["stem"])
							#stem = '\'' if stem == '`' else stem
							word_map[word].append(stem)
							word_map_3[word].append(stem)

							if(stem == ""):
								stem = "<EMPTY>"
						if(a[sentence_id][word_id]["suffixes"]):
							a[sentence_id][word_id]["suffixes"] = normalize(a[sentence_id][word_id]["suffixes"])
							for word_abc in a[sentence_id][word_id]["suffixes"].split('+'):
								word_map[word].append(word_abc)
								postfix = postfix + word_abc
							word_map_3[word].append(postfix)
					
						new_recreate_stem_line = new_recreate_stem_line + stem + ' '
						new_recreate_word_line = new_recreate_word_line + word + ' '

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

