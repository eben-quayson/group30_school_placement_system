#include "placement_system/placement_system.cpp"
#include "placement_system/placement_system.hpp"
#include "school_system/SchoolSystem.cpp"
#include "school_system/SchoolSystem.hpp"
#include "utils/utils.cpp"

namespace Globals {
/* Declaring two global vectors of type `Student` and `RegistedSchool`
 * respectively. */
std::vector<Student> template_students;
std::vector<RegistedSchool> template_schools;

/* A global variable that is used to store the default courses that
are offered by the schools. */
std::vector<Programme> default_programmes;

std::vector<std::string> default_programme_names = {
    "Computer Engineering", "Electrical Engineering", "Computer Science",
    "Mechanical Engineering"};

/**
 * It returns a reference to the vector of strings that is the default school
 * courses
 *
 * @return A reference to the vector of strings.
 */
std::vector<Programme> &set_default_school_programmes() {
  for (auto programme_name : default_programme_names) {
    Programme nwProgramme(programme_name, 15);
    default_programmes.push_back(nwProgramme);
  }
  return default_programmes;
}

/* Creating three objects of the class `RegistedSchool` and assigning them to
the variables `KNUST`, `Legon` and `Uenr` respectively. */
RegistedSchool KNUST("KNUST", set_default_school_programmes());
RegistedSchool Legon("UG-Legon", set_default_school_programmes());
RegistedSchool Uenr("UENR", set_default_school_programmes());

/**
 * It reads the student database,
 * creates a student object for each student, and adds the student to the global
 * vector of students
 */
void preload_student_data() {
  std::string fp = "./students.txt";
  auto student_records = util::read_records_from_database(fp);
  std::cout << "\nReading from " << fp << "\n";

  for (auto record : student_records) {
    std::unordered_map<std::string, std::string> student_grades;

    // Split the name of the student from the grades obtained
    auto split_name_from_grades = util::split_by(record, '-');

    // Assign the student name and grades to string and pair objects
    std::string student_name = split_name_from_grades[0];
    auto split_grades = util::split_by(split_name_from_grades[1], ',');

    // Iterate over the pair iterable `split_grades`, append them to
    // student_grades
    for (auto item : split_grades) {
      auto course_and_grade = util::split_by(item, ':');
      student_grades.emplace(course_and_grade[0], course_and_grade[1]);
    }
    // create student objects and add them to a collection of present
    // students.
    Student nwStudent(student_name, student_grades, Globals::template_schools,
                      {"Computer Engineering", "Computer Science"});
    Globals::template_students.push_back(nwStudent);
  }
}

/**
 * It takes a list of schools and pushes them into a vector
 */
void preload_registedschools() {
  for (auto item : {Globals::KNUST, Globals::Legon, Globals::Uenr})
    Globals::template_schools.push_back(item);
}

}  // namespace Globals

void demonstration_1() {
  // call preload function to alter the global variables related to Registed
  // Schools
  Globals::preload_registedschools();

  // call preload function to load data from `students.txt` and parse them
  // into the globals
  Globals::preload_student_data();

  std::cout << "\nDisplaying the student data of the first 3 people from "
               "../students.txt\n\n";
  for (int n = 0; n < 3; ++n) Globals::template_students[n].display_profile();

  std::cout << "\n\n";

  // Create a placement_system object called WAEC
  PlacementSystem WAEC(Globals::template_schools, Globals::template_students);
  // Place students using system member functions
  WAEC.place_students();
  // Display the student who have been placed in a particular University by
  // name Default University can be inferenced from the global variables at
  // the top of this source. {KNUST, UG-Legon, UENR}.
  WAEC.display_placement_results("KNUST");
}

void demonstration_2() {
  // Demonstration of how each Model class works with its member functions
  // And how they all relate to each other when building a system around them.
  std::cout << "\nCreated a Programme Object:\n";
  Programme SocialScience("Political Science", 14);
  std::cout << "Programme Name: " << SocialScience.get_programme_name() << "\n";
  std::cout << "Programme cut_off: " << SocialScience.get_programme_cut_off()
            << "\n\n";

  // Creating a school object called KNUST and adding a programme to it.
  std::cout << "Created a RegistedSchool Object:\n";
  RegistedSchool KNUST("Kwame Nkrumah University of Science and Technology",
                       {SocialScience});
  std::cout << KNUST.get_name() << '\n';
  std::cout << "Programmes the University offers\n";
  for (auto prg : KNUST.get_offered_programmes()) {
    std::cout << "Programme Name: " << prg.get_programme_name() << "\n";
    std::cout << "Programme cut_off: " << prg.get_programme_cut_off() << "\n";
  }
  std::cout << "\n\n";

  // Creating the student object and calling its member functions.
  std::cout << "Created a Student Object:\n";
  Student nwStd("Prince Kwabena Appiah", {{"Mathematics", "A"}}, {KNUST},
                {"Political Science"});
  nwStd.display_profile();

  /* Creating a placement system object called Waec, adding a school
  and a student to it, placing the student, displaying the placement results,
  getting the placed students, getting the rejected students, getting the
  registered schools and getting the students. */
  std::cout << "Created a placement_system Object:\n";

  PlacementSystem Waec({KNUST}, {nwStd});
  for (auto student : Waec.get_students())
    std::cout << "Student '" << student.get_name() << "' is in the System.\n";

  for (auto school : Waec.get_registed_schools())
    std::cout << "Registed School '" << school.get_name()
              << "' is in the System.\n";

  Waec.place_students();
  Waec.display_placement_results(
      "Kwame Nkrumah University of Science and Technology");

  std::cout << "\n\n";

  // Creating a school object called Legon and a student object called
  // tempStd and adding the student object as a student to the Waec
  // placement_system.
  std::cout << "Creating new RegistedSchool Object.\n";
  RegistedSchool Legon("University of Ghana, Legon", {SocialScience});

  std::cout << "Creating another Student object.\n";
  Student tempStd("Theresa", {{"Mathematics", "A"}}, {Legon},
                  {"Political Science"});

  std::cout << "Adding Student object to new RegistedSchool Object.\n\n";
  Waec.add_school(Legon);
  Waec.add_student(tempStd);

  Waec.place_students();
  Waec.display_placement_results("University of Ghana, Legon");
}

int main() {
  std::cout
      << "Placement System project by Group 30.\n\nPress 1 or 2 to run "
         "demonstration_1 or demonstration_2 "
         "respectively.\n\ndemonstration_1 "
         "parses information about students from `students.txt` and "
         "incorporates the data into "
         "the Placement System Models defined in the "
         "project.\nCheck `preload_registedschools()` and "
         "`preload_student_data()` in main.cpp to understand what goes under "
         "the hood.\n\ndemonstration_2 creats objects in the function "
         "scope and exercises how one would use the member functions of "
         "each class.\n\n Choose which one you wish to run: ";
  int response;
  std::cin >> response;

  if (response == 1)
    demonstration_1();
  else if (response == 2)
    demonstration_2();
  else
    std::cout << "Please enter a valid response next time!";

  return 0;
}