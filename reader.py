import os
import sys


class Reader:
    def read_in_buffer(self, file_object, buffer_size):
        '''
        Lazy function (generator) to read a file piece by piece.
        '''

        while True:
            data = file_object.read(buffer_size)
            if not data:
                break
            yield data

    def read_file(self, buffer_size=1024, file_name=None):

        '''
        Define buffer_size is 1024, file_name is None
        Here should add expection
        This method will call different parser according to the file name
        '''

        input_file = open(file_name)
        # use getattr to build factory method pattern
        # use splitext to spearate the filename and extension name
        method = getattr(self, "parser_" + os.path.splitext(file_name)[0] + "_data")
        for piece in self.read_in_buffer(input_file, buffer_size):
            method(piece)

    def parser_actors_data(self, piece_data):
        #do parser in here
        print 'parser_actors_data'
        pass

    def parser_actresses_data(self, piece_data):
        #do parser in here
        print 'parser_actresses_data'
        pass

    def parser_ratings_data(self, piece_data):
        #do parser in here
        print 'parser_ratings_data'
        pass

    def parser_editors_data(self, piece_data):
        #do parser in here
        print 'parser_editors_data'
        pass

    def parser_keywords_data(self, piece_data):
        print 'parser_editors_data'
        pass

if __name__ == "__main__":
    buffer_size = int(sys.argv[1])
    file_name = sys.argv[2]
    test_reader = Reader()
    test_reader.read_file(buffer_size, file_name)
