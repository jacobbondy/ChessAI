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
import time as t;

chessBoard = bitboard.Board()

Board = chessBoard.getBoard()

class ChessBoard(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        
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
        self.canvas.bind("<Button-2>", self.clear)
        
        for row in range (8):
            for column in range(8):
                for piece, bitboard in Board.items():
                    if bitboard[row][column]:
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
                
                #Moving piece in board dictionary
                self.pieces[row,column] = self.pieces.pop(self.piece_position)
                    
                #Reset important variables
                self.is_grabbed = False
                self.current_piece = None
                self.piece_position = None

    def drawMoves(self, column, row):
        position = (column + row*8)
        
        moves = chessBoard.getMove(position)
        
        for i in range(8):
            for j in range(8):
                if moves[i][j] == 1:
                    rect = self.board[i,j]
                    self.canvas.itemconfig(rect, fill="#ffcccc")
                    
    def clear(self, event):
        for i in range(8):
            for j in range(8):
                rect = self.board[i,j]
                self.canvas.itemconfig(rect, fill=self.original_colors[i,j])
                

if __name__ == "__main__":
    chessboard = ChessBoard("Chess")
    chessboard.mainloop()