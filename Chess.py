# -*- coding: utf-8 -*-
"""
Created on Sat Oct  5 21:47:24 2024

If you need to recreate the environment for some reason
Uninstall python in environment then run:
conda install pytorch::pytorch=*=*cuda* spyder

python version: 3.10.15
pytorch version: 1.12.1 Build py3.10_cuda11.3_cudnn8_0

@author: Jacob Bondy
"""

import torch, tkinter as tk
from PIL import ImageTk, Image
import bitboard
import time as t

cb = bitboard.Board()
cb.clearSquare(0)


class ChessBoard(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        
        self.chessBoard = bitboard.Board()
        
        self.rows = 8
        self.columns = 8
        self.cell_width = 60
        self.cell_height = 60
        
        self.is_grabbed = False
        self.current_piece = None
        self.piece_position = None
        
        self.canvas = tk.Canvas(self, width=self.columns*self.cell_width, height=self.rows*self.cell_height, borderwidth=0, highlightthickness=0)
        self.canvas.pack(side="top", fill="both", expand="true")
        
        self.board = {}
        self.original_colors = {}
        self.pieces = {}
        self.piece_images = {
            'K': ImageTk.PhotoImage(Image.open("./Pieces/White/King.png")),
            'Q': ImageTk.PhotoImage(Image.open("./Pieces/White/Queen.png")),
            'R': ImageTk.PhotoImage(Image.open("./Pieces/White/Rook.png")),
            'B': ImageTk.PhotoImage(Image.open("./Pieces/White/Bishop.png")),
            'N': ImageTk.PhotoImage(Image.open("./Pieces/White/Knight.png")),
            'P': ImageTk.PhotoImage(Image.open("./Pieces/White/Pawn.png")),
            'k': ImageTk.PhotoImage(Image.open("./Pieces/Black/King.png")),
            'q': ImageTk.PhotoImage(Image.open("./Pieces/Black/Queen.png")),
            'r': ImageTk.PhotoImage(Image.open("./Pieces/Black/Rook.png")),
            'b': ImageTk.PhotoImage(Image.open("./Pieces/Black/Bishop.png")),
            'n': ImageTk.PhotoImage(Image.open("./Pieces/Black/Knight.png")),
            'p': ImageTk.PhotoImage(Image.open("./Pieces/Black/Pawn.png"))
        }
        
        
        for column in range(self.columns):
            for row in range(self.rows):
                x1 = column * self.cell_width
                y1 = row * self.cell_height
                x2 = x1 + self.cell_width
                y2 = y1 + self.cell_height
                color = "#779952" if (row + column) % 2 == 0 else "#edeed1"
                self.board[row, column] = self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, tags="rect")
                self.original_colors[row, column] = color  # Store original color

        self.canvas.bind("<Button-1>", self.on_square_click)
        self.canvas.bind("<Button-2>", self.updateBoard)
        self.canvas.bind("<Button-3>", self.on_square_right_click)
        
        for row in range (8):
            for column in range(8):
                for piece, bb in self.chessBoard.getBoard().items():
                    if bb[row][column]:
                        self.pieces[row, column] = \
                            self.canvas.create_image(self.cell_width*column, self.cell_height*row, image=self.piece_images[piece], anchor = tk.NW, tag = piece)
                        

                        
    def on_square_click(self, event):
        # Get the coordinates of the click
        x, y = event.x, event.y
        
        # Find the row and column based on the click
        column = x // self.cell_width
        row = y // self.cell_height
        
        print(f"Column: {column} Row: {row}, Position: {row*8 + column}")
        
        if (row, column) in self.board:
            
            # Check if the piece is grabbed
            if not self.is_grabbed and (row, column) in self.pieces:
                # Store the image ID and position, as well as raise it to the front
                self.current_piece = self.pieces[row,column]
                self.canvas.tag_raise(self.current_piece)
                self.is_grabbed = True  # Set grabbed state to True
                self.piece_position = (row, column)
                
                self.drawMoves(column, row)
                
                
            elif self.is_grabbed:
                #Places the piece down in current square
                self.canvas.coords(self.current_piece, column*self.cell_width, row*self.cell_height)
                
                #Capturing
                if (row, column) in self.pieces and (row, column) != self.piece_position:
                    self.canvas.delete(self.pieces.pop((row,column)))
                    self.chessBoard.clearSquare(row*8 + column)
                
                #Moving piece in board dictionary
                self.pieces[row,column] = self.pieces.pop(self.piece_position)
                self.chessBoard.move(self.piece_position[0]*8 +self.piece_position[1], row*8 + column)
                    
                #Reset important variables
                self.is_grabbed = False
                self.current_piece = None
                self.piece_position = None
                self.clear()
    
    def on_square_right_click(self, event):
        # Get the coordinates of the click
        x, y = event.x, event.y
        
        # Find the row and column based on the click
        column = x // self.cell_width
        row = y // self.cell_height
        
        if (row, column) in self.board and (row, column) in self.pieces:
            self.chessBoard.clearSquare(row*8 + column)
            self.canvas.delete(self.pieces.pop((row,column)))

    def drawMoves(self, column, row):
        position = (column + row*8)
        
        moves = self.chessBoard.getMove(position)
        
        for i in range(8):
            for j in range(8):
                if moves[i][j] == 1:
                    rect = self.board[i,j]
                    self.canvas.itemconfig(rect, fill="#ffcccc")
    
    def updateBoard(self, event):
        Board = self.chessBoard.getBoard()
        self.is_grabbed = False
        self.current_piece = None
        self.piece_position = None
        self.clear()
        
        for piece in self.pieces.values():
            self.canvas.delete(piece)
        
        self.pieces = {}
        
        for row in range (8):
            for column in range(8):
                for piece, bb in Board.items():
                    if bb[row][column]:
                        self.pieces[row, column] = \
                            self.canvas.create_image(self.cell_width*column, self.cell_height*row, image=self.piece_images[piece], anchor = tk.NW, tag = piece)
                    
    def clear(self):
        for i in range(8):
            for j in range(8):
                rect = self.board[i,j]
                self.canvas.itemconfig(rect, fill=self.original_colors[i,j])
                

if __name__ == "__main__":
    chessboard = ChessBoard("Chess")
    chessboard.mainloop()