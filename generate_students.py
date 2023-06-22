# The below code is creating a list of students and their courses and grades.
# And adds the randomized list to the end a file going to serve the purpose of
# the program database.

from random import choice
from defaults import student_names, grades, courses

with open("./students.txt", 'w') as file:
  grouped_students = []
  for name in student_names:
    student_courses = []
    for course in courses:
      student_courses.append(f"{course}:{choice(grades)}")
    
    new_student = name + "- " + ", ".join(student_courses)  + "\n"
    grouped_students.append(new_student)

  for data in grouped_students:
    file.write(data)
  
