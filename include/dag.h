/**
 * 有向无环图数据结构定义
*/
#ifndef __DAG__H__
#define __DAG__H__

#include <stdbool.h>
#include <stdlib.h>

typedef struct _Vertex Vertex;

typedef struct _Edge {
    Vertex *from;
    Vertex *to;
    struct _Edge *next;
} Edge;

typedef struct _dag_vertex_t {
    int id;
    Edge *out_edges;
    int in_degree;
    bool visited;
} dag_vertex_t;

typedef struct {
    Vertex **verticles;
    int num_vertices;
} DAG;

DAG* DAG_create(int num_vertices);
void DAG_destroy(DAG* dag);
Vertex *DAG_add_vertex(DAG *dag, int id);
bool DAG_add_edge(DAG *dag, Vertex *from, Vertex *to);
bool DAG_del_edge(DAG *dag, Edge *edge);
bool DAG_is_dag(DAG *dag);

#endif