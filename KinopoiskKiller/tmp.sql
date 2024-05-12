# add users
INSERT INTO Users (UserID, Username, Password, Email) VALUES (1, 'user1', 'password1', 'user1@example.com');
INSERT INTO Users (UserID, Username, Password, Email) VALUES (2, 'user2', 'password2', 'user2@example.com');
INSERT INTO Users (UserID, Username, Password, Email) VALUES (3, 'user3', 'password3', 'user3@example.com');
INSERT INTO Users (UserID, Username, Password, Email) VALUES (4, 'user4', 'password4', 'user4@example.com');
INSERT INTO Users (UserID, Username, Password, Email) VALUES (5, 'user5', 'password5', 'user5@example.com');

# add movies
INSERT INTO Movies (MovieID, PathImage, Name, Description) VALUES (1, '/images/The_Wolf_of_Wall_Street_photo-resizer.ru.jpg', 'The Wolf of Wall Street', 'Фильм о жизни брокера Уолл-стрит');
INSERT INTO Movies (MovieID, PathImage, Name, Description) VALUES (2, '/images/The_Karate_Kid.png', 'The Karate Kid', 'Фильм о молодом парне, изучающем каратэ');
INSERT INTO Movies (MovieID, PathImage, Name, Description) VALUES (3, '/images/Good_Will_Hunting.jpeg', 'Good Will Hunting', 'Фильм о математическом гении');
INSERT INTO Movies (MovieID, PathImage, Name, Description) VALUES (4, '/images/The_Princess_Bride.jpg', 'The Princess Bride', 'Фильм о приключениях и любви');
INSERT INTO Movies (MovieID, PathImage, Name, Description) VALUES (5, '/images/Forrest_Gump.jpg', 'Forrest Gump', 'Фильм о жизни Форреста Гампа');

INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/Green.jpg', 'Зеленая миля', 'Фильм о доброте и милосердии');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/runout.jpeg', 'Побег из Шоушенка', 'Фильм о дружбе и надежде');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/leon.jpg', 'Леон', 'Фильм о необычной дружбе');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/begin.jpeg', 'Начало', 'Фильм о воровстве снов');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/inter.jpeg', 'Интерстеллар', 'Фильм о космосе и времени');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/oneplusone.jpeg', '1+1', 'Фильм о дружбе и взаимопомощи');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/prestine.jpg', 'Престиж', 'Фильм о магии и соперничестве');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/braingame.jpeg', 'Игры разума', 'Фильм о гениальности и интригах');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/otec.jpg', 'Крестный отец', 'Фильм о мафиозной семье');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/boysclub.jpeg', 'Бойцовский клуб', 'Фильм о бунте и самопознании');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/Matrix.jpg', 'Матрица', 'Фильм о виртуальной реальности');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/black.jpeg', 'Темный рыцарь', 'Фильм о супергероях и злодеях');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/truman.jpeg', 'Шоу Трумана', 'Фильм о реальности и вымысле');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/ring.png', 'Властелин колец', 'Фильм о фантазии и приключениях');
INSERT INTO Movies (PathImage, Name, Description) VALUES ('/images/glad.jpg', 'Гладиатор', 'Фильм о мести и славе');

SELECT Movies.*, AVG(GradeMovie.Grade) AS AverageGrade
FROM Movies
JOIN CollectionsMovie ON Movies.MovieID = CollectionsMovie.MovieID
JOIN Collections ON Collections.CollectionID = CollectionsMovie.CollectionID
LEFT JOIN GradeMovie ON Movies.MovieID = GradeMovie.MovieID
WHERE Collections.Name = :CollectionName
GROUP BY Movies.MovieID;