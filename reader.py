import os
import sys


class Reader:

    def read_file(self, file_name=None):

        '''
        Define file_name is None
        Here should add expection
        This method will call different parser according to the file name
        Use getattr to build factory method pattern
        Use splitext to spearate the filename and extension name
        '''

        method = getattr(self, "parser_" + os.path.splitext(file_name)[0] + "_data")
        method(file_name)

    def parser_actors_data(self, file_name):
        #do parser in here
        with open(file_name, 'r') as input_file:
                for line in input_file:
                    if line.count("THE ACTORS LIST"):

                        break
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
    file_name = sys.argv[1]
    test_reader = Reader()
    test_reader.read_file(file_name)
