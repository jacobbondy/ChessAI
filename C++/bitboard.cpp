#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <cstdint>
#include <iostream>
#include <math.h>

//Run this to create wheel file: python setup.py bdist_wheel
//Then install with pip: pip install ./dist/<file>.whl --force-reinstall

using Bitboard = uint64_t;

class Board {
    public:
        Bitboard whitePawns;
        Bitboard whiteRooks;
        Bitboard whiteKnights;
        Bitboard whiteBishops;
        Bitboard whiteQueen;
        Bitboard whiteKing;
        Bitboard whitePieces;

        Bitboard blackPawns;
        Bitboard blackRooks;
        Bitboard blackKnights;
        Bitboard blackBishops;
        Bitboard blackQueen;
        Bitboard blackKing;
        Bitboard blackPieces;

        Bitboard allPieces;

        Bitboard knightMoves[64];
        Bitboard kingMoves[64];

    Board(){
        // Initialize the board to the starting position

        whitePawns = 0x000000000000FF00;
        whiteRooks = 0x0000000000000081;
        whiteKnights = 0x0000000000000042;
        whiteBishops = 0x0000000000000024;
        whiteQueen = 0x0000000000000010;
        whiteKing = 0x0000000000000008;
        updateWhitePieces(&whitePieces);

        blackPawns = 0x00FF000000000000;
        blackRooks = 0x8100000000000000;
        blackKnights = 0x4200000000000000;
        blackBishops = 0x2400000000000000;
        blackQueen = 0x1000000000000000;
        blackKing = 0x0800000000000000;
        updateBlackPieces(&blackPieces);


        //used to empty board for testing
        // whitePawns = 0;
        // whiteRooks = 0;
        // whiteKnights = 0;
        // whiteBishops = 0;
        // whiteQueen = 0;
        // whiteKing = 0;
        // updateWhitePieces(&whitePieces);

        // blackPawns = 0;
        // blackRooks = 0;
        // blackKnights = 0;
        // blackBishops = 0;
        // blackQueen = 0;
        // blackKing = 0;
        // updateBlackPieces(&blackPieces);


        generateKnightMoves(knightMoves);
        generateKingMoves(kingMoves);
        allPieces = whitePieces | blackPieces;
        
    }

    void updateWhitePieces (Bitboard* board){
        *board = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueen | whiteKing;
    }

    void updateBlackPieces (Bitboard* board){
        *board = blackPawns | blackRooks | blackKnights | blackBishops | blackQueen | blackKing;
    }

    PyObject* getKnightMove(int position){
        return bitboardToArray(knightMoves[position]);
    }

    PyObject* getKingMove(int position){
        return bitboardToArray(kingMoves[position]);
    }

    PyObject* getRookMove(int position){
        Bitboard totalMoves = 0;
        Bitboard curPos = 1ULL << (63-position);
        int tempPos = position;

        //Calculating right sliding
        while (tempPos % 8 < 7){
            tempPos += 1;
            curPos = curPos >> 1;
            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        //check if piece is on the edge, otherwise add to move list
        if (!(tempPos == position)) totalMoves |= curPos;

        tempPos = position;
        curPos = 1ULL << (63-position);

        //Calculating left sliding
        while (tempPos % 8 > 0){
            tempPos -= 1;
            curPos = curPos << 1;
            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        //check if piece is on the edge, otherwise add to move list
        if (!(tempPos == position)) totalMoves |= curPos;


        tempPos = position;
        curPos = 1ULL << (63-position);

        //Calculating up sliding
        while (tempPos > 7){
            tempPos -= 8;
            curPos = curPos << 8;

            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        //check if piece is on the edge, otherwise add to move list
        if (!(tempPos == position)) totalMoves |= curPos;

        tempPos = position;
        curPos = 1ULL << (63-position);

        //Calculating down sliding
        while (tempPos < 56){
            tempPos += 8;
            curPos = curPos >> 8;

            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        return bitboardToArray(totalMoves);
    }

    PyObject* getBishopMove(int position){
        Bitboard totalMoves = 0;
        Bitboard curPos = 1ULL << (63-position);
        int tempPos = position;

        //up left sliding
        while (tempPos > 7 && tempPos % 8 > 0){
            tempPos -= 9;
            curPos = curPos << 9;

            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        //check if piece is on the edge, otherwise add to move list
        if (!(tempPos == position)) totalMoves |= curPos;

        tempPos = position;
        curPos = 1ULL << (63-position);

        //up right sliding
        while (tempPos > 7 && tempPos % 8 < 7){
            tempPos -= 7;
            curPos = curPos << 7;

            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        //check if piece is on the edge, otherwise add to move list
        if (!(tempPos == position)) totalMoves |= curPos;

        tempPos = position;
        curPos = 1ULL << (63-position);

        //down left sliding
        while (tempPos < 56 && tempPos % 8 > 0){
            tempPos += 7;
            curPos = curPos >> 7;

            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        //check if piece is on the edge, otherwise add to move list
        if (!(tempPos == position)) totalMoves |= curPos;

        tempPos = position;
        curPos = 1ULL << (63-position);

        //down right sliding
        while (tempPos < 56 && tempPos % 8 < 7){
            tempPos += 9;
            curPos = curPos >> 9;

            //if there is no piece on current square, add it to move list and continue
            if (!(curPos & allPieces)){
                totalMoves |= curPos;
            }
            //if there is a piece on current square, add it to move list and break
            //will be checked later if friendly piece
            else{
                totalMoves |= curPos;
                break;
            }
        }

        //check if piece is on the edge, otherwise add to move list
        if (!(tempPos == position)) totalMoves |= curPos;

        return bitboardToArray(totalMoves);
    }

    void generateKnightMoves(Bitboard knightMoves[64]){
        const uint64_t moves[8] = {15, 17, -15, -17,10, 6, -10, -6};
        uint64_t targetSquare;

        for (uint64_t i = 0; i < 64; i++){
            knightMoves[i] = 0x0000000000000000;

            for (uint64_t move: moves){
                targetSquare = 0x0000000000000000;
                //Edge of board cases
                if (((i % 8 == 0 || i % 8 == 1) && (move == 6 || move == -10)) ||
                    ((i % 8 == 0) && (move == -17 || move == 15 )) ||
                    ((i % 8 == 6 || i % 8 == 7) && (move == -6 || move == 10)) ||
                    ((i % 8 == 7) && (move == 17 || move == -15))){
                        continue;
                    }

                uint64_t temp = move + i;

                if (temp >= 0 && temp < 64){
                    targetSquare |= (1ULL << (63-temp));
                    knightMoves[i] |= targetSquare;
                } 
                else{
                    continue;
                }
            }
        }
    }

    void generateKingMoves(Bitboard kingMoves[64]){
        const uint64_t moves[8] = {1, -1, -8, 8, -7, -9, 7, 9};
        uint64_t targetSquare;

        for (uint64_t i = 0; i < 64; i++){
            kingMoves[i] = 0x0000000000000000;

            for (uint64_t move: moves){
                targetSquare = 0x0000000000000000;
                //Edge of board cases
                if (((i % 8 == 0) && (move == -1 || move == -9 || move == 7)) ||
                    ((i % 8 == 7) && (move == 1 || move == -7 || move == 9))){
                        continue;
                    }

                uint64_t temp = move + i;

                if (temp >= 0 && temp < 64){
                    targetSquare |= (1ULL << (63-temp));
                    kingMoves[i] |= targetSquare;
                } 
                else{
                    continue;
                }
            }
        }        

    }

    PyObject* getBoard(){
        char pieces[6] = {'K','Q', 'R', 'B', 'N', 'P'};
        char piece[2];

        piece[1] = '\0';

        PyObject* board = PyDict_New();
        if (!board) return nullptr;

        for (int i = 0; i < 12; i++){
                piece[0] = pieces[i%6] + 32*((int) i/6);

                PyObject* bitboard = getBitboard(piece);

                if (!bitboard){
                    Py_DECREF(board); // Decrement reference count for the outer dictionary
                    return nullptr; // Return nullptr on failure
                }
                PyObject* pieceString = PyUnicode_FromString(piece);
                if (!pieceString) {
                    PyErr_Print();
                    Py_DECREF(board); // Clean up
                    Py_DECREF(bitboard); // Clean up
                    return nullptr;
                }

                // Set the item in the dictionary
                if (PyDict_SetItem(board, pieceString, bitboard) < 0) {
                    PyErr_Print();
                    Py_DECREF(board); // Clean up
                    Py_DECREF(pieceString); // Clean up
                    Py_DECREF(bitboard); // Clean up
                    return nullptr; // Return nullptr on failure
                }

                // Clean up reference counts
                Py_DECREF(pieceString); // Decrement the reference count for pieceString
                Py_DECREF(bitboard); // Decrement the reference count for bitboard
            }
        return board;
    }

    PyObject* getBitboard(char* piece){
        Bitboard bitboard;

        switch (piece[0]) {
            case 'K': // White King
                bitboard = whiteKing;
                break;
            case 'Q': // White Queen
                bitboard = whiteQueen;
                break;
            case 'R': // White Rook
                bitboard = whiteRooks;
                break;
            case 'B': // White Bishop
                bitboard = whiteBishops;
                break;
            case 'N': // White Knight
                bitboard = whiteKnights;
                break;
            case 'P': // White Pawn
                bitboard = whitePawns;
                break;
            case 'k': // Black King
                bitboard = blackKing;
                break;
            case 'q': // Black Queen
                bitboard = blackQueen;
                break;
            case 'r': // Black Rook
                bitboard = blackRooks;
                break;
            case 'b': // Black Bishop
                bitboard = blackBishops;
                break;
            case 'n': // Black Knight
                bitboard = blackKnights;
                break;
            case 'p': // Black Pawn
                bitboard = blackPawns;
                break;
            default:
                // Handle unknown piece
                throw std::invalid_argument("Unknown piece character.");
        }

        return bitboardToArray(bitboard);
    }

    PyObject* bitboardToArray(Bitboard bitboard){
        PyObject* outerList = PyList_New(8);
        if (!outerList) return nullptr;

        for (int rank = 7; rank >= 0; rank--){
            PyObject* innerList = PyList_New(8);
            if (!innerList){
                Py_DECREF(outerList);
                return nullptr;
            }

            for (int file = 7; file >= 0; file--){
                int value = (bitboard & (1ULL << (rank * 8 + file))) ? 1:0;
                PyObject* pyValue = PyLong_FromLong(value);
                if (!pyValue){
                    Py_DECREF(outerList);
                    Py_DECREF(innerList);
                    return nullptr;
                }
                PyList_SET_ITEM(innerList, 7-file, pyValue);
            }

            PyList_SET_ITEM(outerList, 7-rank, innerList);
        }
        return outerList;
    }
    

};

// Function to be called from Python
extern "C" {

    //Need to define struct to create python object
    typedef struct {
        PyObject_HEAD
        Board* board;  // Pointer to the C++ Board object
    } PyBoard;

     static void PyBoard_dealloc(PyBoard* self) {
        delete self->board;  // Clean up C++ object
        Py_TYPE(self)->tp_free((PyObject*)self);  // Free Python object
    }

    static PyObject* PyBoard_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
        PyBoard* self = (PyBoard*)type->tp_alloc(type, 0);
        if (self != nullptr) {
            self->board = new Board();  // Create a new Board instance
        }
        return (PyObject*)self;
    }

    static PyObject* PyBoard_getKnightMove(PyBoard* self, PyObject* args){
        int position;

        if (!PyArg_ParseTuple(args, "i", &position)){
            return nullptr;
        }

        return self->board->getKnightMove(position);
    }

    static PyObject* PyBoard_getKingMove(PyBoard* self, PyObject* args){
        int position;

        if (!PyArg_ParseTuple(args, "i", &position)){
            return nullptr;
        }

        return self->board->getKingMove(position);
    }

    static PyObject* PyBoard_getRookMove(PyBoard* self, PyObject* args){
        int position;

        if (!PyArg_ParseTuple(args, "i", &position)){
            return nullptr;
        }

        return self->board->getRookMove(position);
    }

    static PyObject* PyBoard_getBishopMove(PyBoard* self, PyObject* args){
        int position;

        if (!PyArg_ParseTuple(args, "i", &position)){
            return nullptr;
        }

        return self->board->getBishopMove(position);
    }

    //Method to return bitboard for given piece
    static PyObject* PyBoard_getBitboard(PyBoard* self, PyObject* args){
        char* piece;

        if (!PyArg_ParseTuple(args, "s", &piece)){
            return nullptr;
        }

        return self->board->getBitboard(piece);
    }

    static PyObject* PyBoard_getBoard(PyBoard* self, PyObject* args){
        return self->board->getBoard();
    }

    // Method definitions
    static PyMethodDef PyBoard_methods[] = {
        // {"print", (PyCFunction)PyBoard_print, METH_NOARGS, "Print the chess board."},
        {"getBitboard", (PyCFunction)PyBoard_getBitboard, METH_VARARGS, "Return bitboard corresponding to piece."},
        {"getBoard", (PyCFunction)PyBoard_getBoard, METH_NOARGS, "Return array of bitboards representing board."},
        {"getKnightMove", (PyCFunction)PyBoard_getKnightMove, METH_VARARGS, "Return array of knight moves from position (0-63)"},
        {"getKingMove", (PyCFunction)PyBoard_getKingMove, METH_VARARGS, "Return array of king moves from position (0-63)"},
        {"getRookMove", (PyCFunction)PyBoard_getRookMove, METH_VARARGS, "Return array of rook moves from position (0-63)"},
        {"getBishopMove", (PyCFunction)PyBoard_getBishopMove, METH_VARARGS, "Return array of bishop moves from position (0-63)"},
        {nullptr, nullptr, 0, nullptr}  // Sentinel
    };

    static PyTypeObject PyBoardType = {
        PyVarObject_HEAD_INIT(nullptr, 0)
        "bitboard.Board",             // tp_name
        sizeof(PyBoard),              // tp_basicsize
        0,                            // tp_itemsize
        (destructor)PyBoard_dealloc, // tp_dealloc
        0,                            // tp_print
        0,                            // tp_getattr
        0,                            // tp_setattr
        0,                            // tp_reserved
        0,                            // tp_repr
        0,                            // tp_as_number
        0,                            // tp_as_sequence
        0,                            // tp_as_mapping
        0,                            // tp_hash
        0,                            // tp_call
        0,                            // tp_str
        0,                            // tp_getattro
        0,                            // tp_setattro
        0,                            // tp_as_buffer
        Py_TPFLAGS_DEFAULT,          // tp_flags
        "Board objects",              // tp_doc
        0,                            // tp_traverse
        0,                            // tp_clear
        0,                            // tp_richcompare
        0,                            // tp_weaklistoffset
        0,                            // tp_iter
        0,                            // tp_iternext
        PyBoard_methods,              // tp_methods
        0,                            // tp_members
        0,                            // tp_getset
        0,                            // tp_base
        0,                            // tp_dict
        0,                            // tp_descr_get
        0,                            // tp_descr_set
        0,                            // tp_dictoffset
        0,                            // tp_init
        0,                            // tp_alloc
        PyBoard_new,                  // tp_new
    };

     // Module definition
    static struct PyModuleDef bitboardmodule = {
        PyModuleDef_HEAD_INIT,
        "bitboard",  // Module name
        nullptr,     // Module documentation
        -1,          // Size of per-interpreter state of the module
        nullptr      // Note: No methods for the module itself, only the class
    };

    // Module initialization function
    PyMODINIT_FUNC PyInit_bitboard(void) {
        PyObject* module = PyModule_Create(&bitboardmodule);
        if (module == nullptr) return nullptr;

        // Add the Board type to the module
        if (PyType_Ready(&PyBoardType) < 0) return nullptr;
        Py_INCREF(&PyBoardType);
        PyModule_AddObject(module, "Board", (PyObject*)&PyBoardType);  // Add Board to module

        return module;
    }
}
