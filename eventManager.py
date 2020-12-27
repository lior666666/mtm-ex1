#### IMPORTS ####
#import event_manager as EM

#### PART 1 ####
def isValidSemester(semester):
    return (semester > 0)

def isValidAge(age):
    return (age >= 16) and (age <= 120)

def isValidYearOfBirth(age, year):
    return (2020 - age == year)

def isValidIdNmber(id_number):
    digits_list = [int(digit) for digit in str(id_number)]
    return (len(digits_list) == 8) and (digits_list[0] != 0)

def isValidName(name):
    words_list = name.split()
    for word in words_list:
        if word.isalpha() == False:
            return False
    return True

def studentPrint(target_file, strip_student_list):
    for i, element in enumerate(strip_student_list):
        if i == len(strip_student_list)-1:
            target_file.write(element)
        else:
            target_file.write(element + ", ")
    target_file.write("\n")

def addIdNumberToList(students_id_list, id_number):
    for i, current_id_number in enumerate(students_id_list):
        if i != len(students_id_list)-1:
            if current_id_number > id_number:
                students_id_list.insert(i, id_number)
                return i
    students_id_list.append(id_number)
    return len(students_id_list)-1

# Filters a file of students' subscription to specific event:
#   orig_file_path: The path to the unfiltered subscription file
#   filtered_file_path: The path to the new filtered file
def fileCorrect(orig_file_path: str, filtered_file_path: str):
    origin_file = open(orig_file_path, 'r')
    target_file = open(filtered_file_path, 'w')
    students_id_list = []
    valid_students_list = []
    for student in origin_file:
        student_list = student.split(',')
        strip_student_list = [i.strip() for i in student_list]
        id_number = int(strip_student_list[0])
        strip_student_list[1] = " ".join(strip_student_list[1].split())
        name = strip_student_list[1]
        age = int(strip_student_list[2])
        year = int(strip_student_list[3])
        semester = int(strip_student_list[4])
        if isValidName(name) and isValidIdNmber(id_number) and isValidSemester(semester) and isValidAge(age) and isValidYearOfBirth(age, year):
            student_index = addIdNumberToList(students_id_list, id_number)
            valid_students_list.insert(student_index, strip_student_list)

    for i, id_number in enumerate(students_id_list):
        if i == len(students_id_list)-1:
            studentPrint(target_file, valid_students_list[i])
        elif id_number != students_id_list[i+1]:
            studentPrint(target_file, valid_students_list[i])
    origin_file.close()
    target_file.close()  
  
# Writes the names of the K youngest students which subscribed 
# to the event correctly.
#   in_file_path: The path to the unfiltered subscription file
#   out_file_path: file path of the output file
def printYoungestStudents(in_file_path: str, out_file_path: str, k: int) -> int:
    pass
    #TODO
    
    
# Calculates the avg age for a given semester
#   in_file_path: The path to the unfiltered subscription file
#   retuns the avg, else error codes defined.
def correctAgeAvg(in_file_path: str, semester: int) -> float:
    pass
    #TODO
    

#### PART 2 ####
# Use SWIG :)
# print the events in the list "events" using the functions from hw1
#   events: list of dictionaries
#   file_path: file path of the output file
def printEventsList(events :list,file_path :str): #em, event_names: list, event_id_list: list, day: int, month: int, year: int):
    pass
    #TODO   
    
    
def testPrintEventsList(file_path :str):
    events_lists=[{"name":"New Year's Eve","id":1,"date": EM.dateCreate(30, 12, 2020)},\
                    {"name" : "annual Rock & Metal party","id":2,"date":  EM.dateCreate(21, 4, 2021)}, \
                                 {"name" : "Improv","id":3,"date": EM.dateCreate(13, 3, 2021)}, \
                                     {"name" : "Student Festival","id":4,"date": EM.dateCreate(13, 5, 2021)},    ]
    em = printEventsList(events_lists,file_path)
    for event in events_lists:
        EM.dateDestroy(event["date"])
    EM.destroyEventManager(em)

#### Main #### 
# feel free to add more tests and change that section. 
# sys.argv - list of the arguments passed to the python script
if __name__ == "__main__":
    import sys
    if len(sys.argv)>1:
        testPrintEventsList(sys.argv[1])
