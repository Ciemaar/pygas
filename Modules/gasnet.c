#include "Python.h"

#define GASNET_SEQ
#include "gasnet.h"

static PyObject *
py_gasnet_init(PyObject *self, PyObject *args)
{
    int argc = 5;
    char **argv = (char**) malloc(argc * sizeof(char*));
    argv[0] = "2";
    argv[1] = "3";
    argv[2] = "4";
    argv[3] = "5";
    argv[4] = "6";

    int status = gasnet_init(&argc, &argv);
    return Py_BuildValue("i", status);
}

static PyObject *
py_gasnet_attach(PyObject *self, PyObject *args)
{
    int status = gasnet_attach(NULL, 0, gasnet_getMaxLocalSegmentSize(), GASNET_PAGESIZE);
    return Py_BuildValue("i", status);
}

static PyObject *
py_gasnet_exit(PyObject *self, PyObject *args)
{
    int ok;
    int exitcode = 0;
    ok = PyArg_ParseTuple(args, "|i", &exitcode);

    gasnet_exit(exitcode);

    Py_RETURN_NONE;
}

static PyObject *
py_gasnet_mynode(PyObject *self, PyObject *args)
{
    gasnet_node_t rank = gasnet_mynode();
    return Py_BuildValue("i", rank);
}

static PyObject *
py_gasnet_nodes(PyObject *self, PyObject *args)
{
    gasnet_node_t ranks = gasnet_nodes();
    return Py_BuildValue("i", ranks);
}

static PyObject *
py_gasnet_getenv(PyObject *self, PyObject *args)
{
    int ok;
    const char *key, *value;
    ok = PyArg_ParseTuple(args, "s", &key);

    value = gasnet_getenv(key);

    return Py_BuildValue("s", value);
}

static PyObject *
py_gasnet_barrier_wait(PyObject *self, PyObject *args)
{
    int ok;
    int id = 0;
    int flags = GASNET_BARRIERFLAG_ANONYMOUS;
    ok = PyArg_ParseTuple(args, "|ii", &id, &flags);

    gasnet_barrier_wait(id, flags);

    Py_RETURN_NONE;
}

static PyObject *
py_gasnet_barrier_notify(PyObject *self, PyObject *args)
{
    int ok;
    int id = 0;
    int flags = GASNET_BARRIERFLAG_ANONYMOUS;
    ok = PyArg_ParseTuple(args, "|ii", &id, &flags);

    gasnet_barrier_notify(id, flags);

    Py_RETURN_NONE;
}

static PyObject *
py_gasnet_barrier_try(PyObject *self, PyObject *args)
{
    int ok;
    int id = 0;
    int flags = GASNET_BARRIERFLAG_ANONYMOUS;
    ok = PyArg_ParseTuple(args, "|ii", &id, &flags);

    gasnet_barrier_try(id, flags);

    Py_RETURN_NONE;
}

static PyMethodDef gasnet_methods[] = {
    {"init",           py_gasnet_init,           METH_VARARGS, "Bootstrap GASNet job."},
    {"exit",           py_gasnet_exit,           METH_VARARGS, "Terminate GASNet runtime."},
    {"attach",         py_gasnet_attach,         METH_NOARGS,  "Initialize and setup node."},
    {"nodes",          py_gasnet_nodes,          METH_VARARGS, "Number of nodes in job."},
    {"mynode",         py_gasnet_mynode,         METH_VARARGS, "Index of current node in job."},
    {"getenv",         py_gasnet_getenv,         METH_VARARGS, "Query environment when job was spawned."},
    {"barrier_notify", py_gasnet_barrier_notify, METH_VARARGS, "Execute notify to split-phase barrier."},
    {"barrier_wait",   py_gasnet_barrier_wait,   METH_VARARGS, "Execute notify to split-phase barrier."},
    {"barrier_try",    py_gasnet_barrier_try,    METH_VARARGS, "Execute notify to split-phase barrier."},
    {NULL,             NULL}           /* sentinel */
};

PyMODINIT_FUNC
initgasnet(void)
{
    Py_InitModule3("gasnet", gasnet_methods, "Interface to GASNet.");
}
