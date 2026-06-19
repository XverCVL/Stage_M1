CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -fopenmp -fno-omit-frame-pointer

ROOT = /media/hdd_0/Programmation/SolverCO

SRC_DIR = $(ROOT)/src

BUILD_LIB = $(ROOT)/build/lib
COLPACK_LIB = $(ROOT)/external_libraries/ColPack/lib
KAMIS_LIB = $(ROOT)/external_libraries/KaMIS/lib
STABLESOLVER_LIB = $(ROOT)/external_libraries/StableSolver/lib

LDFLAGS := -L$(BUILD_LIB) -L$(COLPACK_LIB) -L$(KAMIS_LIB) -L$(STABLESOLVER_LIB) -L/usr/local/lib

SOLVER_CO_LIBS = \
	-lvcp_lib \
	-lw_vcp_lib \
	-lmax_clique_lib \
	-lw_max_clique_lib \
	-lmis_lib \
	-lw_mis_lib \
	-lgraph_lib \
	-lbenchmark_lib

COLPACK_LIBS = -lColPack

KAMIS_LIBS = \
	-llibonlinemis \
	-llibfiles \
	-llibkaffpa

STABLESOLVER_LIBS = \
	-lStableSolver_clique_greedy \
	-lStableSolver_clique_local_search \
	-lStableSolver_clique \
	-lStableSolver_stable_greedy \
	-lStableSolver_stable_large_neighborhood_search \
	-lStableSolver_stable_local_search_row_weighting \
	-lStableSolver_stable_local_search \
	-lStableSolver_stable \
	-loptimizationtools_graph \
	-loptimizationtools_utils \

SYSTEM_LIBS = \
	-llapack \
	-lblas \
	-lz \
	-lbz2

LIBS = $(SOLVER_CO_LIBS) $(COLPACK_LIBS) $(KAMIS_LIBS) $(STABLESOLVER_LIBS) $(SYSTEM_LIBS)

INCLUDES = \
    -I$(SRC_DIR) \
    -I$(SRC_DIR)/solvers/graph_solvers \
    -I$(SRC_DIR)/solvers/graph_solvers/components \
    -I$(SRC_DIR)/solvers/graph_solvers \
    -I$(SRC_DIR)/solvers/graph_solvers/color_vertices/vcp \
    -I$(SRC_DIR)/solvers/graph_solvers/color_vertices/vcp/vcp_solvers \
    -I$(SRC_DIR)/solvers/graph_solvers/color_vertices/vcp/vcp_solvers/vcp_tabu \
    -I$(SRC_DIR)/solvers/graph_solvers/color_vertices/vcp/vcp_solvers/vcp_colpack \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/max_clique \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/max_clique/clique_solvers \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/max_clique/clique_solvers/clique_clisat \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/max_clique/clique_solvers/clique_stablesolver \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/mis \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/mis/mis_solvers/mis_greedy \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/mis/mis_solvers/mis_kamis \
    -I$(SRC_DIR)/solvers/graph_solvers/select_vertices/mis/mis_solvers/mis_stablesolver \
    -I$(SRC_DIR)/utils/benchmark \
    -I$(SRC_DIR)/utils/containers \
    -I$(SRC_DIR)/utils/graph \
    -I$(ROOT)/external_libraries/StableSolver/include

app: app.cpp
	$(CXX) -o app app.cpp $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS)
