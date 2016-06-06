/**
 *  C言語によるアルゴリズム辞典
 *  P.316
 *  B木 B-Tree
 */

#include <stdio.h>
#include <stdlib.h>

/* 1ページのデータ数の上限の半分 */
#define M 2

/* 探索キーの型 */
typedef int keytype;

/* FALSE=0, TRUE=1 */
typedef enum {FALSE, TRUE} boolean;

/* ページの定義 */
typedef struct page {
    /* データ数 */
    int n;
    /* キー */
    keytype key[2 * M];
    /* 他ページへのポインタ */
    struct page *branch[2 * M + 1];
} *pageptr; /* pageptr はページへのポインタ型 */

/* B 木の根 */
pageptr root = NULL;
/* キー */
keytype key;
/* 論理型の変数 */
boolean done, deleted, undersize;
/* insert() の生成した新しいページ */
pageptr newp;
/* 関数の返すメッセージ */
char *message;

/* 新しいページの生成 */
pageptr newpage(void)
{
    pageptr p;

    if ((p = malloc(sizeof *p)) == NULL) {
        printf("メモリ不足.\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

/* キー key を B 木から探す */
void search(void)
{
    pageptr p;
    int k;

    p = root;
    while (p != NULL) {
        k = 0;
        while (k < p->n && p->key[k] < key) k++;
        if (k < p->n && p->key[k] == key) {
            message = "見つかりました";
            return;
        }
        p = p->branch[k];
    }
    message = "見つかりません";
}

/* key を p->key[k] に挿入する */
void insertitem(pageptr p, int k)
{
    int i;

    for (i = p->n; i > k; i--) {
        p->key[i] = p->key[i - 1];
        p->branch[i + 1] = p->branch[i];
    }
    
    p->key[k] = key;
    p->branch[k + 1] = newp;
    p->n++;
}

/* key を p->key[k] に挿入し、ページ p を割る */
void split(pageptr p, int k)
{
    int j, m;
    pageptr q;

    // 小さい方のPageの割った後の要素数
    m = (k <= M) ? M : M + 1;
    
    q = newpage();
    for (j = m + 1; j <= 2 * M; j++) {
        q->key[j - m - 1] = p->key[j - 1];
        q->branch[j - m] = p->branch[j];
    }
    
    q->n = 2 * M - m;
    p->n = m;
    
    if (k <= M) {
        insertitem(p, k);
    } else {
        insertitem(q, k - m);
    }
    
    // ひとつ上のPageに移動させるため 小さい方のPageの一番大きい要素を消す
    key = p->key[p->n - 1];
    q->branch[0] = p->branch[p->n];
    p->n--;
    
    /* 新しいページを newp に入れて戻る */
    newp = q;
}

/* ページ p から木を再帰的にたどって挿入する */
void insertsub(pageptr p)
{
    int k;

    if (p == NULL) {
        done = FALSE;
        newp = NULL;
        return;
    }
    
    k = 0;
    while (k < p->n && p->key[k] < key) k++;
    
    if (p->key[k] == key) {
        message = "もう登録されています";
        done = TRUE;
        return;
    }
    
    insertsub(p->branch[k]);
    
    if (done) {
        return;
    }
    
    if (p->n < 2 * M) {
        /* ページが割れない場合 */
        insertitem(p, k);
        done = TRUE;
    } else {
        /* ページが割れる場合 */
        split(p, k);
        done = FALSE;
    }
}

/* キー key を B 木に挿入する */
void insert(void)
{
    pageptr p;

    message = "登録しました";
    
    insertsub(root);
    
    if (done) {
        return;
    } else {
      // ひとつ目の場合 と split の場合
      p = newpage();
      p->n = 1;
      p->key[0] = key;
      p->branch[0] = root;
      p->branch[1] = newp;
      root = p;
    }
}

/* p->kkey[k], p->branch[k+1] を外す。ページが小さくなりすぎたら undersize フラグを立てる。 */
void removeitem(pageptr p, int k)
{
    while (++k < p->n) {
        p->key[k - 1] = p->key[k];
        p->branch[k] = p->branch[k + 1];
    }
    undersize = --(p->n) < M;
}

/* p->branch[k-1] の最右要素を p->key[k-1] 経由で p->branch[k] に動かす */
void moveright(pageptr p, int k)
{
    int j;
    pageptr left, right;

    left = p->branch[k - 1];
    right = p->branch[k];
    for (j = right->n; j > 0; j--) {
        right->key[j] = right->key[j - 1];
        right->branch[j + 1] = right->branch[j];
    }
    right->branch[1] = right->branch[0];
    right->n++;
    
    // 下に移行してる
    right->key[0] = p->key[k - 1];
    
    p->key[k - 1] = left->key[left->n - 1];
    right->branch[0] = left->branch[left->n];
    left->n--;
}

/* p->branch[k] の最左要素を p->key[k-1] 経由で p->branch[k-1] に動かす */
void moveleft(pageptr p, int k)
{
    int j;
    pageptr left, right;

    left = p->branch[k - 1];
    right = p->branch[k];
    left->n++;
    left->key[left->n - 1] = p->key[k - 1];
    left->branch[left->n] = right->branch[0];
    p->key[k - 1] = right->key[0];
    right->branch[0] = right->branch[1];
    right->n--;
    for (j = 1; j <= right->n; j++) {
        right->key[j - 1] = right->key[j];
        right->branch[j] = right->branch[j + 1];
    }
}

/* p->branch[k-1], p->branch[k] を統合する */
void combine(pageptr p, int k)
{
    int j;
    pageptr left, right;

    right = p->branch[k];
    left = p->branch[k-1];
    
    left->n++;
    left->key[left->n - 1] = p->key[k - 1];
    left->branch[left->n] = right->branch[0];
    
    for (j = 1; j <= right->n; j++) {
        left->n++;
        left->key[left->n - 1] = right->key[j - 1];
        left->branch[left->n] = right->branch[j];
    }
    removeitem(p, k - 1);
    free(right);
}

/* 小さくなりすぎたページ p->branch[k] を修復する */
void restore(pageptr p, int  k)
{
    undersize = FALSE;
    if (k > 0) {
        if (p->branch[k - 1]->n > M) {
            moveright(p, k);
        } else {
            combine(p, k);
        }
    } else {
        if (p->branch[1]->n > M) {
            moveleft(p, 1);
        } else {
            combine(p, 1);
        }
    }
}

/* ページ p から再帰的に木をたどり削除する */
void deletesub(pageptr p)
{
    int k;
    pageptr q;

    if (p == NULL) {
        /* 見つからなかった */
        return;
    }
    k = 0;
    while (k < p->n && p->key[k] < key) k++;
    if (k < p->n && p->key[k] == key) {
        /* 見つかった */
        deleted = TRUE;
        if ((q = p->branch[k + 1]) != NULL) {
            while (q->branch[0] != NULL) q = q->branch[0];
            p->key[k] = key = q->key[0];
            deletesub(p->branch[k + 1]);
            if (undersize) {
                restore(p, k + 1);
            }
        } else {
            removeitem(p, k);
        }
    } else {
        deletesub(p->branch[k]);
        if (undersize) {
            restore(p, k);
        }
    }
}

/* キー key を B 木から外す */
void delete(void)
{
    pageptr p;

    deleted = undersize = FALSE;
    /* 根から再帰的に木をたどり削除する */
    deletesub(root);
    if (deleted) {
        if (root->n == 0) {
            /* 根が空になった場合 */
            p = root;
            root = root->branch[0];
            free(p);
        }
        message = "削除しました";
    } else {
        message = "見つかりません";
    }
}

/* デモ用に B 木を表示する */
void printtree(pageptr p)
{
    static int depth = 0;
    int k;

    if (p == NULL) {
        printf(".");
        return;
    }
    printf("("); depth++;
    for (k = 0; k < p->n; k++) {
        printtree(p->branch[k]); /* 再帰呼び出し */
        printf("%d", p->key[k]);
    }
    printtree(p->branch[p->n]); /* 再帰呼び出し */
    printf(")"); depth--;
}

#include <ctype.h>

int main()
{
    char s[2];

    for (;;) {
        printf("挿入 In, 検索 Sn, 削除 Dn (n:整数) ? ");
        if (scanf("%1s%d", s, &key) != 2) {
            break;
        }
        switch (s[0]) {
        case 'I':
        case 'i':
            insert();
            break;
        case 'S':
        case 's':
            search();
            break;
        case 'D':
        case 'd':
            delete();
            break;
        default:
            message = "???";
            break;
        }
        printf("%s\n\n", message);
        printtree(root);
        printf("\n\n");
    }
    return EXIT_SUCCESS;
}
