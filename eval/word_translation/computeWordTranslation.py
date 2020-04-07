import sys, pandas, csv, os, pickle, codecs
#from scipy import spatial
import argparse, numpy, os
from sklearn.metrics.pairwise import cosine_similarity
from scipy.spatial.distance import cdist, pdist, squareform
from timeit import default_timer as timer
from scipy.stats.mstats import rankdata

def compute(cos_scores,df_dic_source_target,params,out):
    #score #2
    total = cos_scores.shape[1]-1
    sorted_scores= numpy.argsort(numpy.argsort(cos_scores,axis=1))#rankdata(cos_scores,axis=1)-1
    
    #2nd method
    diag = numpy.diagonal(cos_scores)
    #max_scores = cos_scores.max(axis=1)
    max_index = numpy.where(sorted_scores==total)[1]
    max_scores = [cos_scores[idx,d]for idx,d in enumerate(max_index)]
    top_index = numpy.where(sorted_scores==total-9)[1]
    top_scores = [cos_scores[idx,d]for idx,d in enumerate(top_index)]
    dscores = [1 if d==max_scores[idx] else 0 for idx,d in enumerate(diag)] #compares actual value not just rank
    dtopscores = [1 if d>=top_scores[idx] else 0 for idx,d in enumerate(diag)]
    df_dic_source_target['p1']=dscores
    df_dic_source_target['p10']=dtopscores
    
    p1 = df_dic_source_target['p1'].mean()
    p10 = df_dic_source_target['p10'].mean()

    
    if params.case:
        out_file= "case-sensitive_"+os.path.basename(params.emb)
    else:
        out_file= "case-insensitive_"+os.path.basename(params.emb)
    if "unq" in params.dic:
        out_file = "unq_{}".format(out_file)
    else:
        out_file = "multi_{}".format(out_file)
    df_dic_source_target.to_csv(os.path.dirname(params.emb) + "/" + out_file+out+'.out',sep='\t',columns=['source','target','p1','p10'],index=False,encoding='utf-8')
    return [p1,p10]

def main():
    STDERR_OUT = ""
    parser = argparse.ArgumentParser(description='Word translation score')
    parser.add_argument("--emb", type=str, default="", help="Path to embedding")
    parser.add_argument("--dic", type=str, default="", help="Path to dictionary")
    parser.add_argument("--prefix", type=str, default="", help="Language")
    parser.add_argument("--out",type=str,default="",help="Output directory")
    parser.add_argument("--case",type=bool,default=False,help="Case sensitive")
    
    parser.add_argument("--merge_two_embeddings",type=bool,default=False,help="merge_two_embeddings")
    parser.add_argument("--embedding_pr",type=str,default="",help="merge_two_embeddings")
    parser.add_argument("--embedding_en",type=str,default="",help="merge_two_embeddings")

    params = parser.parse_args()

    qualifier = "unq" if "unq" in params.dic else "multi"

    if(params.case):
        IDENTIFIER = "{}:\t{}\t{}\t".format("Iteration", qualifier, "cAsE")
    else:
        IDENTIFIER = "{}:\t{}\t{}\t".format("Iteration", qualifier, "case")
    print(IDENTIFIER, end='\t', file=sys.stdout, flush=True)
    print(sys.argv, file=sys.stdout, flush=True)

    if(params.merge_two_embeddings):
        print(IDENTIFIER + "Merging two embeddings", file=sys.stdout, flush=True)
        assert os.path.isfile(params.embedding_pr)
        assert os.path.isfile(params.embedding_en)

        f_emb = codecs.open(params.emb, 'w', 'utf-8-sig')
        ### Do line handling better ###
        f_emb.write(
            str(int(codecs.open(params.embedding_pr, 'r', 'utf-8-sig').readline().strip().split()[0]) + \
            int(codecs.open(params.embedding_en, 'r', 'utf-8-sig').readline().strip().split()[0])))
        f_emb.write(" " + codecs.open(params.embedding_pr, 'r', 'utf-8-sig').readline().strip().split()[1] + "\n")
        ### Line handling done ###

        cnt_prefix = False
        with codecs.open(params.embedding_pr, 'r', 'utf-8-sig') as read_f:
            for line in read_f:
                if cnt_prefix is False:
                    cnt_prefix = True
                    continue;
                f_emb.write(params.prefix + ":" + line)

        cnt_en = False
        with codecs.open(params.embedding_en, 'r', 'utf-8-sig') as read_f:
            for line in read_f:
                if cnt_en is False:
                    cnt_en = True
                    continue;
                f_emb.write("en:" + line)
        f_emb.close()

    assert os.path.isfile(params.emb)
    assert os.path.isfile(params.dic)
    we_name = os.path.basename(params.emb)
    dic_name = os.path.basename(params.dic)
    df_we= pandas.read_csv(params.emb, skiprows=1, sep="\s+",header=None, quoting=csv.QUOTE_NONE,encoding='utf-8')
    df_di= pandas.read_csv(params.dic, sep="\t",header=None, quoting=csv.QUOTE_NONE,engine='python',encoding='utf-8')#\s+
    if params.case: #case sensitive
        df_dic=df_di
    else:
        df_dic = df_di.applymap(str.lower)
        df_we[0]=df_we[0].str.lower()

    df_we[0].replace('SPACE_', '').replace('_', ' ')
    # df_we[0].loc[df_we[0].startswith('SPACE_'), 'my_channel'] =
    # df_we[0] = (df_we[0].startswith('SPACE_')).str.replace('SPACE_', '')

    STDERR_OUT = STDERR_OUT + "dict_size:\t{}\t".format(len(df_dic))
    df_dic.drop_duplicates(inplace=True)
    STDERR_OUT = STDERR_OUT + "final_dict_size:\t{}\t".format(len(df_dic))
    STDERR_OUT = STDERR_OUT + "we_size:\t{}\t".format(len(df_we))
    df_we.drop_duplicates([0],inplace=True)
    STDERR_OUT = STDERR_OUT + "final_we_size:\t{}\t".format(len(df_we))
    cnames= ['dim'+str(c) for c in df_we.columns]
    cnames[0]='word'
    col_names=",".join(cnames)
    df_we.columns=col_names.split(',')
    df_dic.columns=['source','target']
    df_dic_source=pandas.merge(df_dic,df_we,left_on='source',right_on='word')
    df_dic_source_target=pandas.merge(df_dic_source,df_we,left_on='target',right_on='word',suffixes=('_src', '_trg'))
#   df_dic[~df_dic['source'].isin(df_dic_source_target['source'])]
#   df_dic[~df_dic['target'].isin(df_dic_source_target['target'])]
    column_names=df_dic_source_target.columns
    source_column_names=[c for c in column_names if c.endswith('src')]
    target_column_names=[c for c in column_names if c.endswith('trg')]
    cos_scores = 1-cdist(df_dic_source_target[source_column_names[1:]], df_dic_source_target[target_column_names[1:]], 'cosine')
    total = cos_scores.shape[1]-1
    coverage = float(total+1)/len(df_dic)
    # print("{}:\t{};\t{}".format(total+1, len(df_dic), coverage), file=sys.stderr, flush=True)
    
    [p1_s2t,p10_s2t] = compute(cos_scores,df_dic_source_target,params,'S2T')
    [p1_t2s,p10_t2s] = compute(numpy.transpose(cos_scores),df_dic_source_target,params,'T2S')
    p1_avg = numpy.array([p1_s2t, p1_t2s]).mean()
    p10_avg = numpy.array([p10_s2t, p10_t2s]).mean()
    STDERR_OUT = IDENTIFIER + "S2T\tP@1\t{:.5f}\tP@10\t{:.5f}\tCoverage\t{:.5f}\tMutual\t{}".format(p1_s2t,p10_s2t,coverage, total+1)
    STDERR_OUT = IDENTIFIER + "T2S\tP@1\t{:.5f}\tP@10\t{:.5f}\tCoverage\t{:.5f}\tMutual\t{}\t".format(p1_t2s,p10_t2s,coverage, total+1) + "\n" + STDERR_OUT
    STDERR_OUT = IDENTIFIER + "AVG\tP@1\t{:.5f}\tP@10\t{:.5f}\tCoverage\t{:.5f}\tMutual\t{}\t".format(p1_avg,p10_avg,coverage, total+1) + "\n" + STDERR_OUT
    print(STDERR_OUT, file=sys.stderr, flush=True)

if __name__ == "__main__":
    start = timer()
    main()
    end = timer()
    #print "Execution time: "
    #print(end - start)  
