import java.sql.*;


//DB001 Project1 12161579_박준현
public class main {
    public static void main(String[] args) throws Exception {

        try {
            Class.forName("org.postgresql.Driver");//1
        } catch (ClassNotFoundException e) {
            System.out.println("Where is your PostgreSQL JDBC Driver? Include in your library path!");
            e.printStackTrace();
            return;
        }
        System.out.println("PostgreSQL JDBC Driver Registered!");
        /// if you have a error in this part, check jdbc driver(.jar file)

        Connection connection = null;

        try {
            connection = DriverManager.getConnection(
                    "jdbc:postgresql://127.0.0.1:5432/project_movie", "postgres", "cse3207");//2
        } catch (SQLException e) {
            System.out.println("Connection Failed! Check output console");
            e.printStackTrace();
            return;
        }
        /// if you have a error in this part, check DB information (db_name, user name, password)

        if (connection != null) {
            System.out.println(connection);
            System.out.println("You made it, take control your database now!\n");
        } else {
            System.out.println("Failed to make connection!");
        }
        
        try {
	        //statement 오브젝터 이용해서 query생성, 실행
	        Statement st = connection.createStatement();
	        
	        //RESET
        	//st.executeUpdate(DELETEData());
	        //System.out.println("All tables DELETEd.");


        	//Create table
	        st.executeUpdate(createTable());
	        System.out.println("Tables created.");
	        
	        //initialize data
	        st.executeUpdate(initialData());
	        System.out.println("Initial data inserted.");
	        
	        
	        //actor
	        //2.1. Winona Ryder won the “Best supporting actor” award in 1994
	    	st.executeUpdate("INSERT INTO actorObtain VALUES (4095, 199495, 1994);"
	    			+ "INSERT INTO award VALUES (199495, 'Best supporting actor');"); 
	    	
	        ResultSet r1 = null;
	        int actorID;
	        int awardID;
	        int year;
	        String awardName;
	        System.out.println("\nStatement: Winona Ryder won the “Best supporting actor” award in 1994");
	        try {
				r1 = st.executeQuery("SELECT * FROM actorObtain;");
		        System.out.println("Translated SQL: INSERT INTO actorObtain VALUES ((SELECT actorID FROM actor WHERE actorName = 'Winona Ryder'), 199495, 1994);");
		        System.out.println("Translated SQL: SELECT * FROM actorObtain;");
		        System.out.println("==========[actorObtain]==========");
		        System.out.println("actorID, awardID, year");
		        System.out.println("----------------------");
		    	while(r1.next()) {
		    		actorID = r1.getInt("actorID");
		    		awardID = r1.getInt("awardID");
		    		year = r1.getInt("year");
		    		System.out.println(actorID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r1.close();
			}
	        
	        ResultSet r2 = null;
	        try {
	        	r2 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (199495, 'Best supporting actor');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r2.next()) {
		    		awardID = r2.getInt("awardID");
		    		awardName = r2.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r2.close();
			}
	        
	    	
	    	
	    	
	        
	        //actor
	    	//2.2. Andrew Garfield won the “Best supporting actor” award in 2011 
	        System.out.println("\nStatement: Andrew Garfield won the “Best supporting actor” award in 2011");
	        st.executeUpdate("INSERT INTO actorObtain VALUES ((SELECT actorID FROM actor WHERE actorName = 'Andrew Garfield'), 201103, 2011);"
	        		+ "INSERT INTO award VALUES (201103, 'Best supporting actor');");
			ResultSet r3 = null;
	        try {
	        	r3 = st.executeQuery("SELECT * FROM actorObtain;");
	        	System.out.println("Translated SQL: INSERT INTO actorObtain VALUES ((SELECT actorID FROM actor WHERE actorName = 'Andrew Garfield'), 201103, 2011);");
		        System.out.println("Translated SQL: SELECT * FROM actorObtain;");
		        System.out.println("==========[actorObtain]==========");
		        System.out.println("actorID, awardID, year");
		        System.out.println("----------------------");
		        while(r3.next()) {
		        	actorID = r3.getInt("actorID");
		        	awardID = r3.getInt("awardID");
		    		year = r3.getInt("year");
		    		System.out.println(actorID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r3.close();
			}
	        
	        ResultSet r4 = null;
	        try {
	        	r4 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (201103, 'Best supporting actor');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r4.next()) {
		        	awardID = r4.getInt("awardID");
		    		awardName = r4.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r4.close();
			}
	        
	        
	        //actor
	    	//2.3. Jesse Eisenberg won the “Best main actor” award in 2011 
	        
	        System.out.println("\nStatement: Jesse Eisenberg won the “Best main actor” award in 2011");
	        st.executeUpdate("INSERT INTO actorObtain VALUES ((SELECT actorID FROM actor WHERE actorName = 'Jesse Eisenberg'), 201132, 2011);"
	        		+ "INSERT INTO award VALUES (201132, 'Best main actor');");
			ResultSet r5 = null;
	        try {
	        	r5 = st.executeQuery("SELECT * FROM actorObtain;");
	        	System.out.println("Translated SQL: INSERT INTO actorObtain VALUES ((SELECT actorID FROM actor WHERE actorName = 'Jesse Eisenberg'), 201132, 2011);");
		        System.out.println("Translated SQL: SELECT * FROM actorObtain;");
		        System.out.println("==========[actorObtain]==========");
		        System.out.println("actorID, awardID, year");
		        System.out.println("----------------------");
		        while(r5.next()) {
		        	actorID = r5.getInt("actorID");
		    		awardID = r5.getInt("awardID");
		    		year = r5.getInt("year");
		    		System.out.println(actorID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r5.close();
			}
	        
	        ResultSet r6 = null;
	        try {
	        	r6 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (201132, 'Best main actor');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r6.next()) {
		        	awardID = r6.getInt("awardID");
		    		awardName = r6.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r6.close();
			}
	        
	        
	        //actor
	    	//2.4. Johnny Depp won the “Best villain actor” award in 2011 
	        //st.executeUpdate("insert into award values(201140, 'Best villain actor')");
	        System.out.println("\nStatement: Johnny Depp won the “Best villain actor” award in 2011");
	        st.executeUpdate("INSERT INTO actorObtain VALUES ((SELECT actorID FROM actor WHERE actorName = 'Johnny Depp'), 201140, 2011);"
	        		+ "INSERT INTO award VALUES (201140, 'Best villain actor');");
			ResultSet r7 = null;
	        try {
	        	r7 = st.executeQuery("SELECT * FROM actorObtain;");
	        	System.out.println("Translated SQL: INSERT INTO actorObtain VALUES ((SELECT actorID FROM actor WHERE actorName = 'Johnny Depp'), 201140, 2011);");
		        System.out.println("Translated SQL: SELECT * FROM actorObtain;");
		        System.out.println("==========[actorObtain]==========");
		        System.out.println("actorID, awardID, year");
		        System.out.println("----------------------");
		        while(r7.next()) {
		        	actorID = r7.getInt("actorID");
		    		awardID = r7.getInt("awardID");
		    		year = r7.getInt("year");
		    		System.out.println(actorID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r7.close();
			}
	        
	        ResultSet r8 = null;
	        try {
	        	r8 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (201140, 'Best villain actor');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r8.next()) {
		        	awardID = r8.getInt("awardID");
		    		awardName = r8.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r8.close();
			}
	        
	        
	        //movie
	    	//2.5. Edward Scissorhands won the “Best fantasy movie” award in 1991
	        System.out.println("\nStatement: Edward Scissorhands won the “Best fantasy movie” award in 1991");
	        st.executeUpdate("INSERT INTO movieObtain VALUES ((SELECT movieID FROM movie WHERE movieName = 'Edward Scissorhands'), 199135, 1991);"
	        		+ "INSERT INTO award VALUES (199135, 'Best fantasy movie');");
	        
			ResultSet r9 = null;
			int movieID;
	        try {
	        	r9 = st.executeQuery("SELECT * FROM movieObtain;");
	        	System.out.println("Translated SQL: INSERT INTO movieObtain VALUES ((SELECT movieID FROM movie WHERE movieName = 'Edward Scissorhands'), 199135, 1991);");
		        System.out.println("Translated SQL: SELECT * FROM movieObtain;");
		        System.out.println("==========[movieObtain]==========");
		        System.out.println("movieID, awardID, year");
		        System.out.println("----------------------");
		        while(r9.next()) {
		        	movieID = r9.getInt("movieID");
		        	awardID = r9.getInt("awardID");
		    		year = r9.getInt("year");
		    		System.out.println(movieID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r9.close();
			}
	        
	        ResultSet r10 = null;
	        try {
	        	r10 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (199135, 'Best fantasy movie');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r10.next()) {
		        	awardID = r10.getInt("awardID");
		    		awardName = r10.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r10.close();
			}
	        
	        //movie
	    	//2.6. Alice In Wonderland won the “Best fantasy movie” award in 2011
	        System.out.println("\nStatement: Alice In Wonderland won the “Best fantasy movie” award in 2011");
	        st.executeUpdate("INSERT INTO movieObtain VALUES ((SELECT movieID FROM movie WHERE movieName = 'Alice In Wonderland'), 201190, 2011);"
	        		+ "INSERT INTO award VALUES (201190, 'Best fantasy movie');");
	        
			ResultSet r11 = null;
	        try {
	        	r11 = st.executeQuery("SELECT * FROM movieObtain;");
	        	System.out.println("Translated SQL: INSERT INTO movieObtain VALUES ((SELECT movieID FROM movie WHERE movieName = 'Alice In Wonderland'), 201190, 2011);");
		        System.out.println("Translated SQL: SELECT * FROM movieObtain;");
		        System.out.println("==========[movieObtain]==========");
		        System.out.println("movieID, awardID, year");
		        System.out.println("----------------------");
		        while(r11.next()) {
		        	movieID = r11.getInt("movieID");
		        	awardID = r11.getInt("awardID");
		    		year = r11.getInt("year");
		    		System.out.println(movieID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r11.close();
			}
	        
	        ResultSet r12 = null;
	        try {
	        	r12 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (201190, 'Best fantasy movie');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r12.next()) {
		        	awardID = r12.getInt("awardID");
		    		awardName = r12.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r12.close();
			}
	        
	        
	        //movie
	    	//2.7. The Dark Knight won the “Best picture” award in 2009 
	        //st.executeUpdate("insert into award values(200945, 'Best picture')");
	        System.out.println("\nStatement: The Dark Knight won the “Best picture” award in 2009");
	        st.executeUpdate("INSERT INTO movieObtain VALUES ((SELECT movieID FROM movie WHERE movieName = 'The Dark Knight'), 200945, 2009);"
	        		+ "INSERT INTO award VALUES (200945, 'Best picture');");
	        
			ResultSet r13 = null;
	        try {
	        	r13 = st.executeQuery("SELECT * FROM movieObtain;");
	        	System.out.println("Translated SQL: INSERT INTO movieObtain VALUES ((SELECT movieID FROM movie WHERE movieName = 'The Dark Knight'), 200945, 2009);");
		        System.out.println("Translated SQL: SELECT * FROM movieObtain;");
		        System.out.println("==========[movieObtain]==========");
		        System.out.println("movieID, awardID, year");
		        System.out.println("----------------------");
		        while(r13.next()) {
		        	movieID = r13.getInt("movieID");
		        	awardID = r13.getInt("awardID");
		    		year = r13.getInt("year");
		    		System.out.println(movieID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r13.close();
			}
	        
	        ResultSet r14 = null;
	        try {
	        	r14 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (200945, 'Best picture');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r14.next()) {
		        	awardID = r14.getInt("awardID");
		    		awardName = r14.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r14.close();
			}
	        
	        
	        //director
	    	//2.8. David Fincher won the “Best director” award in 2011 
	        System.out.println("\nStatement: David Fincher won the “Best director” award in 2011");
	        st.executeUpdate("INSERT INTO directorObtain VALUES ((SELECT directorID FROM director WHERE directorName = 'David Fincher'), 201115, 2011);"
	        		+ "INSERT INTO award VALUES (201115, 'Best director');");
	        
			ResultSet r15 = null;
			int directorID;
	        try {
	        	r15 = st.executeQuery("SELECT * FROM directorObtain;");
	        	System.out.println("Translated SQL: INSERT INTO directorObtain VALUES ((SELECT directorID FROM director WHERE directorName = 'David Fincher'), 201115, 2011);");
		        System.out.println("Translated SQL: SELECT * FROM directorObtain;");
		        System.out.println("==========[directorObtain]==========");
		        System.out.println("directorID, awardID, year");
		        System.out.println("-------------------------");
		        while(r15.next()) {
		        	directorID = r15.getInt("directorID");
		    		awardID = r15.getInt("awardID");
		    		year = r15.getInt("year");
		    		System.out.println(directorID + ", " + awardID + ", " + year);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r15.close();
			}
	        
	        ResultSet r16 = null;
	        try {
	        	r16 = st.executeQuery("SELECT * FROM award;");
	        	System.out.println("\nTranslated SQL: INSERT INTO award VALUES (201115, 'Best director');");
	        	System.out.println("Translated SQL: SELECT * FROM award;");
		        System.out.println("==========[award]==========");
		        System.out.println("awardID, awardName");
		        System.out.println("------------------");
		        while(r16.next()) {
		    		awardID = r16.getInt("awardID");
		    		awardName = r16.getString("awardName");
		    		System.out.println(awardID + ", " + awardName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r16.close();
			}
	        
	        
	        
	        //3.1 Bob rates 5 to “The Dark Knight”. 
	        System.out.println("\nStatement: Bob rates 5 to “The Dark Knight”.");
	        st.executeUpdate("INSERT INTO customerRate VALUES ((SELECT customerID FROM customer WHERE customerName = 'Bob'), (SELECT movieID FROM movie WHERE movieName = 'The Dark Knight'), 5);");
	        
			ResultSet r17 = null;
			int customerID;
			int rate;
	        try {
	        	r17 = st.executeQuery("SELECT * FROM customerRate;");
	        	System.out.println("Translated SQL: INSERT INTO customerRate VALUES ((SELECT customerID FROM customer WHERE customerName = 'Bob'), (SELECT movieID FROM movie WHERE movieName = 'The Dark Knight'), 5);");
		        System.out.println("Translated SQL: SELECT * FROM customerRate;");
		        System.out.println("==========[customerRate]==========");
		        System.out.println("customerID, movieID, rate");
		        System.out.println("-------------------------");
		        while(r17.next()) {
		        	customerID = r17.getInt("customerID");
		    		movieID = r17.getInt("movieID");
		    		rate = r17.getInt("rate");
		    		System.out.println(customerID + ", " + movieID + ", " + rate);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r17.close();
			}
	        
	        //3.2 Bell rates 5 to the movies whose director is “Tim Burton”. 
	        System.out.println("\nStatement: Bell rates 5 to the movies whose director is “Tim Burton”.");
	        st.executeUpdate("INSERT INTO customerRate\r\n" + 
	        		"(customerID, movieID, rate)\r\n" + 
	        		"(SELECT 37, movieID, 5 \r\n" + 
	        		" FROM make natural join director\r\n" + 
	        		" WHERE director.directorName = 'Tim Burton');");
	        
			ResultSet r18 = null;
	        try {
	        	r18 = st.executeQuery("SELECT * FROM customerRate;");
	        	System.out.println("Translated SQL: INSERT INTO customerRate\r\n" + 
	        			"(customerID, movieID, rate)\r\n" + 
	        			"(SELECT (SELECT customerID FROM customer WHERE customerName = 'Bell'), movieID, 5 \r\n" + 
	        			" FROM make natural join director\r\n" + 
	        			" WHERE director.directorName = 'Tim Burton');");
		        System.out.println("Translated SQL: SELECT * FROM customerRate;");
		        System.out.println("==========[customerRate]==========");
		        System.out.println("customerID, movieID, rate");
		        System.out.println("-------------------------");
		        while(r18.next()) {
		        	customerID = r18.getInt("customerID");
		    		movieID = r18.getInt("movieID");
		    		rate = r18.getInt("rate");
		    		System.out.println(customerID + ", " + movieID + ", " + rate);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r18.close();
			}
	        
	        //3.3 Jill rates 4 to the movies whose main actor is female. 
	        System.out.println("\nStatement: Jill rates 4 to the movies whose main actor is female.");
	        st.executeUpdate("insert into customerRate\r\n" + 
	        		"(customerID, movieID, rate)\r\n" + 
	        		"(SELECT 72, movieID, 4\r\n" + 
	        		"FROM casting natural join actor\r\n" + 
	        		"WHERE actor.gender = 'Female' and casting.role = 'Main actor');");
	        
			ResultSet r19 = null;
	        try {
	        	r19 = st.executeQuery("SELECT * FROM customerRate;");
	        	System.out.println("Translated SQL: INSERT INTO customerRate\r\n" + 
	        			"(customerID, movieID, rate)\r\n" + 
	        			"(SELECT (SELECT customerID FROM customer WHERE customerName = 'Jill'), movieID, 4 \r\n" + 
	        			" FROM make NATURAL JOIN director\r\n" + 
	        			" WHERE actor.gender = 'Female' AND casting.role = 'Main actor');");
		        System.out.println("Translated SQL: SELECT * FROM customerRate;");
		        System.out.println("==========[customerRate]==========");
		        System.out.println("customerID, movieID, rate");
		        System.out.println("-------------------------");
		        while(r19.next()) {
		        	customerID = r19.getInt("customerID");
		    		movieID = r19.getInt("movieID");
		    		rate = r19.getInt("rate");
		    		System.out.println(customerID + ", " + movieID + ", " + rate);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r19.close();
			}
	        
	        
	        //3.4 Jack rates 4 to the fantasy movies. 
	        System.out.println("\nStatement: Jack rates 4 to the fantasy movies.");
	        st.executeUpdate("INSERT INTO customerRate\r\n" + 
	        		"(customerID, movieID, rate)\r\n" + 
	        		"(SELECT DISTINCT 83, movieID, 4\r\n" + 
	        		"FROM movieGenre NATURAL JOIN customer\r\n" + 
	        		"WHERE movieGenre.genreName = 'Fantasy');");
	        
			ResultSet r20 = null;
	        try {
	        	r20 = st.executeQuery("SELECT * FROM customerRate;");
	        	System.out.println("Translated SQL: INSERT INTO customerRate\r\n" + 
	        			"(customerID, movieID, rate)\r\n" + 
	        			"(SELECT DISTINCT (SELECT customerID FROM customer WHERE customerName = 'Jack'), movieID, 4\r\n" + 
	        			" FROM movieGenre NATURAL JOIN customer\r\n" + 
	        			" WHERE movieGenre.genreName = 'Fantasy');");
		        System.out.println("Translated SQL: SELECT * FROM customerRate;");
		        System.out.println("==========[customerRate]==========");
		        System.out.println("customerID, movieID, rate");
		        System.out.println("-------------------------");
		        while(r20.next()) {
		        	customerID = r20.getInt("customerID");
		    		movieID = r20.getInt("movieID");
		    		rate = r20.getInt("rate");
		    		System.out.println(customerID + ", " + movieID + ", " + rate);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r20.close();
			}
	        
	        
	        //3.5 John rates 5 to the movies whose director won the “Best director” award 
	        
	        System.out.println("\nStatement: John rates 5 to the movies whose director won the “Best director” award");
	        st.executeUpdate("INSERT INTO customerRate\r\n" + 
	        		"(customerID, movieID, rate)\r\n" + 
	        		"(SELECT 59, movieID, 5\r\n" + 
	        		" FROM make NATURAL JOIN directorObtain\r\n" + 
	        		" WHERE directorID in\r\n" + 
	        		"	(SELECT T.directorID\r\n" + 
	        		"	FROM directorObtain AS T\r\n" + 
	        		"	WHERE T.awardID in (SELECT R.awardID\r\n" + 
	        		"		   FROM award AS R\r\n" + 
	        		"		   WHERE T.awardID = R.awardID AND R.awardName = 'Best director')));");
	        
			ResultSet r21 = null;
	        try {
	        	r21 = st.executeQuery("SELECT * FROM customerRate;");
	        	System.out.println("Translated SQL: INSERT INTO customerRate\r\n" + 
	        			"(customerID, movieID, rate)\r\n" + 
	        			"(SELECT (SELECT customerID FROM customer WHERE customerName = 'John'), movieID, 5\"\r\n" + 
	        			" FROM make NATURAL JOIN directorObtain\r\n" + 
	        			" WHERE directorID in (SELECT T.directorID\r\n" + 
	        			"			FROM directorObtain AS T\r\n" + 
	        			"			WHERE T.awardID in (SELECT R.awardID\r\n" + 
	        			"						FROM award AS R\r\n" + 
	        			"						WHERE T.awardID = R.awardID AND R.awardName = 'Best director')));");
		        System.out.println("Translated SQL: SELECT * FROM customerRate;");
		        System.out.println("==========[customerRate]==========");
		        System.out.println("customerID, movieID, rate");
		        System.out.println("-------------------------");
		        while(r21.next()) {
		        	customerID = r21.getInt("customerID");
		    		movieID = r21.getInt("movieID");
		    		rate = r21.getInt("rate");
		    		System.out.println(customerID + ", " + movieID + ", " + rate);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r21.close();
			}
	        
	      //update avgRates
	        st.executeUpdate("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 17635) WHERE movie.movieID = 17635;");
	        st.executeUpdate("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 22190) WHERE movie.movieID = 22190;");
	        st.executeUpdate("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 32479) WHERE movie.movieID = 32479;");
	        st.executeUpdate("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 83245) WHERE movie.movieID = 83245;");
	        st.executeUpdate("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 11412) WHERE movie.movieID = 11412;");
	        
	        
	        System.out.println("\nStatement: Average rates Updated.");
			ResultSet r22 = null;
			String movieName;
			float avgRate;
	        try {
	        	r22 = st.executeQuery("SELECT movieName, avgRate FROM movie;");
	        	System.out.println("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 17635) WHERE movie.movieID = 17635;");
	        	System.out.println("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 22190) WHERE movie.movieID = 22190;");
	        	System.out.println("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 32479) WHERE movie.movieID = 32479;");
	        	System.out.println("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 83245) WHERE movie.movieID = 83245;");
	        	System.out.println("UPDATE movie SET avgRate = (SELECT AVG(rate) FROM customerRate WHERE movieID = 11412) WHERE movie.movieID = 11412;");
	        	System.out.println("Translated SQL: SELECT movieName, avgRate FROM movie;");
		        System.out.println("==========[movie]==========");
		        System.out.println("movieName, avgRate(real number)");
		        System.out.println("-------------------------------");
		        while(r22.next()) {
		        	movieName = r22.getString("movieName");
		        	avgRate = r22.getFloat("avgRate");
		    		System.out.println(movieName + ", " + avgRate);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r22.close();
			}
	        
	        //QUERIES
	    	
	    	//4. Select the names of the movies whose actor are dead.
	        System.out.println("\nStatement: Select the names of the movies whose actor are dead.");
			ResultSet r23 = null;
	        try {
	        	r23 = st.executeQuery("SELECT movieName FROM movie WHERE movieID IN (SELECT movieID FROM casting NATURAL JOIN actor WHERE dateofdeath IS NOT null);");
	        	System.out.println("Translated SQL: SELECT movieName FROM movie WHERE movieID IN (SELECT movieID FROM casting NATURAL JOIN actor WHERE dateofdeath IS NOT null);");
		        System.out.println("==========[movieName]==========");
		        System.out.println("movieName");
		        System.out.println("---------");
		        while(r23.next()) {
		        	movieName = r23.getString("movieName");
		    		System.out.println(movieName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r23.close();
			}
	        
	    	//5. Select the names of the directors who cast the same actor more than once. 
	        ResultSet r24 = null;
	        String directorName;
	        System.out.println("\nStatement: Select the names of the directors who cast the same actor more than once.");
	        try {
	        	r24 = st.executeQuery("SELECT directorName FROM director WHERE directorID IN (SELECT DISTINCT directorID FROM (make NATURAL JOIN casting) AS T, casting AS S WHERE T.actorID = S.actorID AND NOT (T.movieID = S.movieID));");
	        	System.out.println("Translated SQL: SELECT directorName FROM director WHERE directorID IN (SELECT DISTINCT directorID FROM (make NATURAL JOIN casting) AS T, casting AS S WHERE T.actorID = S.actorID AND NOT (T.movieID = S.movieID));");
		        System.out.println("==========[directorName]==========");
		        System.out.println("directorName");
		        System.out.println("------------");
		        while(r24.next()) {
		        	directorName = r24.getString("directorName");
		    		System.out.println(directorName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r24.close();
			}
	        
	        //6. Select the names of the movies and the genres, WHERE movies have the common genre. 
	        ResultSet r25 = null;
	        System.out.println("\nStatement: Select the names of the movies and the genres, WHERE movies have the common genre.");
			String genreName;
	        try {
	        	r25 = st.executeQuery("SELECT DISTINCT movieName, genreName \r\n" + 
	        			"FROM movie, movieGenre \r\n" + 
	        			"WHERE (movie.movieID, movieGenre.genreName) IN (SELECT DISTINCT T.movieID, T.genreName\r\n" + 
	        			"												FROM (movie NATURAL JOIN movieGenre) AS T, movieGenre AS S\r\n" + 
	        			"												WHERE T.genreName = S.genreName AND NOT T.movieID = S.movieID)\r\n" + 
	        			"												order BY genreName;");
	        	System.out.println("Translated SQL: SELECT DISTINCT movieName, genreName \r\n" + 
	        			"FROM movie, movieGenre \r\n" + 
	        			"WHERE (movie.movieID, movieGenre.genreName) IN (SELECT DISTINCT T.movieID, T.genreName\r\n" + 
	        			"						FROM (movie NATURAL JOIN movieGenre) AS T, movieGenre AS S\r\n" + 
	        			"						WHERE T.genreName = S.genreName AND NOT T.movieID = S.movieID)\r\n" + 
	        			"						order BY genreName;");
		        System.out.println("==========[movie, movieGenre]==========");
		        System.out.println("movieName, genreName(listed in order of genre)");
		        System.out.println("--------------------");
		        while(r25.next()) {
		        	movieName = r25.getString("movieName");
		        	genreName = r25.getString("genreName");
		    		System.out.println(movieName + ", " + genreName);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r25.close();
			}
	        
	        
	        
	        //7. Delete the movies whose director or actor did not get any award and DELETE data FROM related tables. 
			ResultSet r26 = null;
			System.out.println("\nStatement: Delete the movies whose director or actor did not get any award and DELETE data FROM related tables.");
	        try {
	        	st.executeUpdate("DELETE FROM customerRate WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM customerRate WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM movieObtain WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM movieObtain WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM movieGenre WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM movieGenre WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM movie WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM movie WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM make WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM casting WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM make WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM casting WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));");
	        	
	        	System.out.println("DELETE FROM customerRate WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM customerRate WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM movieObtain WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM movieObtain WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM movieGenre WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM movieGenre WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM movie WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM movie WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM make WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM casting WHERE movieID IN (SELECT movieID FROM casting WHERE actorID NOT IN (SELECT actorID FROM actorObtain));\r\n" + 
	        			"DELETE FROM make WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));\r\n" + 
	        			"DELETE FROM casting WHERE movieID IN (SELECT movieID FROM make WHERE directorID NOT IN (SELECT directorID FROM directorObtain));");
	        	r26 = st.executeQuery("SELECT movieID FROM movie;");
	        	System.out.println("\nTranslated SQL: SELECT movieID FROM movie;");
		        System.out.println("==========[movie]==========");
		        System.out.println("movieID");
		        System.out.println("-------");
		        while(r26.next()) {
		        	movieID = r26.getInt("movieID");
		    		System.out.println(movieID);
		    	}
		        r26 = st.executeQuery("SELECT movieID FROM customerRate;");
	        	System.out.println("\nTranslated SQL: SELECT movieID FROM customer WHERE customerID IN (SELECT customerID FROM customerRate);");
		        System.out.println("==========[customerRate]==========");
		        System.out.println("movieID");
		        System.out.println("-------");
		        while(r26.next()) {
		        	movieID = r26.getInt("movieID");
		    		System.out.println(movieID);
		    	}
		        r26 = st.executeQuery("SELECT movieID FROM movieObtain;");
	        	System.out.println("\nTranslated SQL: SELECT movieID FROM movieObtain;");
		        System.out.println("==========[movieObtain]==========");
		        System.out.println("movieID");
		        System.out.println("-------");
		        while(r26.next()) {
		        	movieID = r26.getInt("movieID");
		    		System.out.println(movieID);
		    	}
		        r26 = st.executeQuery("SELECT movieID FROM movieGenre;");
	        	System.out.println("\nTranslated SQL: SELECT movieID FROM movieGenre;");
		        System.out.println("==========[movieGenre]==========");
		        System.out.println("movieID");
		        System.out.println("-------");
		        while(r26.next()) {
		        	movieID = r26.getInt("movieID");
		    		System.out.println(movieID);
		    	}
		        r26 = st.executeQuery("SELECT movieID FROM make;");
	        	System.out.println("\nTranslated SQL: SELECT movieID FROM make;");
		        System.out.println("==========[make]==========");
		        System.out.println("movieID");
		        System.out.println("-------");
		        while(r26.next()) {
		        	movieID = r26.getInt("movieID");
		    		System.out.println(movieID);
		    	}
		        r26 = st.executeQuery("SELECT movieID FROM casting;");
	        	System.out.println("\nTranslated SQL: SELECT actorID FROM casting;");
		        System.out.println("==========[casting]==========");
		        System.out.println("movieID");
		        System.out.println("-------");
		        while(r26.next()) {
		        	movieID = r26.getInt("movieID");
		    		System.out.println(movieID);
		    	}
		        
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally {
				r26.close();
			}
	        
	        //8. Delete all customers and DELETE data FROM related tables. 
	        ResultSet r27 = null;
	        System.out.println("\nStatement: Delete all customers and DELETE data FROM related tables.");
	        try {
	        	st.executeUpdate("DELETE FROM customerRate WHERE customerID IN (SELECT customerID FROM customer);\r\n" + 
	        			"DELETE FROM customer;");
	        	System.out.println("Translated SQL: DELETE FROM customerRate WHERE customerID IN (SELECT customerID FROM customer);\r\n" + 
	        			"DELETE FROM customer;");
	        	System.out.println("==========[customer]==========");
		        System.out.println("customerID");
		        System.out.println("----------");
		        r27 = st.executeQuery("SELECT customerID FROM customer;");
		        while(r27.next()) {
		        	customerID = r27.getInt("customerID"); 
		    		System.out.println(customerID);
		    	}
		        
		        System.out.println("==========[customerRate]==========");
		        System.out.println("customerID");
		        System.out.println("----------");
		        r27 = st.executeQuery("SELECT customerID FROM customerRate;");
		        while(r27.next()) {
		        	customerID = r27.getInt("customerID");
		    		System.out.println(customerID);
		    	}
	        }catch(SQLException e) {
	        	e.printStackTrace();
	        }finally {
	        	r27.close();
	        }
	        
	        //9. Delete all tables and data. 
	        st.executeUpdate(DELETEData());
	        System.out.println("\nDelete all tables and data.");
	        System.out.println("Removal complete.");
	        
	        System.out.println("Closing connection with server.");
	    	connection.close();
        }catch(SQLException e) {
        	e.printStackTrace();
        }
        

    }
    
    public static String DELETEData() {
    	return "DROP SCHEMA public CASCADE;\r\n" + 
    			"CREATE SCHEMA public;\r\n" + 
    			"\r\n" + 
    			"GRANT ALL ON SCHEMA public TO postgres;\r\n" + 
    			"GRANT ALL ON SCHEMA public TO public;";
    }
    public static String initialData() {
    	return "insert into movie\r\n" + 
    			"values(17635, 'Edward Scissorhands', 1991, 06, 29, '20th Century Fox Presents'),\r\n" + 
    			"(22190, 'Alice In Wonderland', 2010, 03, 04, 'Korea Sony Pictures'),\r\n" + 
    			"(32479, 'The Social Network', 2010, 11, 18, 'Korea Sony Pictures'),\r\n" + 
    			"(83245, 'The Dark Knight', 2008, 08, 06, 'Warner Brothers Korea'),\r\n" + 
    			"(11412, 'Dunkirk', 2017, 07, 13, 'Warner Brothers Korea');\r\n" + 
    			"\r\n" + 
    			"insert into director\r\n" + 
    			"values(803, 'Tim Burton', '1958.08.25'),\r\n" + 
    			"(315, 'David Fincher', '1962.08.28'),\r\n" + 
    			"(422, 'Christopher Nolan', '1970.07.30');\r\n" + 
    			"\r\n" + 
    			"insert into actor\r\n" + 
    			"values (3140, 'Johnny Depp', '1963.06.09', null, 'Male'),\r\n" + 
    			"(4095, 'Winona Ryder', '1971.10.29', null, 'Female'),\r\n" + 
    			"(7347, 'Anne Hathaway', '1982.11.12', null, 'Female'),\r\n" + 
    			"(4328, 'Christian Bale', '1974.01.30', null, 'Male'),\r\n" + 
    			"(6810, 'Heath Ledger', '1979.04.04', '2008.1.22', 'Male'),\r\n" + 
    			"(1132, 'Jesse Eisenberg', '1983.10.05', null, 'Male'),\r\n" + 
    			"(8103, 'Andrew Garfield', '1983.08.20', null, 'Male'),\r\n" + 
    			"(7279, 'Fionn Whitehead', '1997.07.18', null, 'Male'),\r\n" + 
    			"(9091, 'Tom Hardy', '1977.09.15', null, 'Male');\r\n" + 
    			"\r\n" + 
    			"insert into customer\r\n" + 
    			"values(14, 'Bob', '1997.11.14', 'Male'),\r\n" + 
    			"(59, 'John', '1978.01.23', 'Male'),\r\n" + 
    			"(83, 'Jack ', '1980.05.04', 'Male'),\r\n" + 
    			"(72, 'Jill ', '1981.04.17', 'Female'),\r\n" + 
    			"(37, 'Bell ', '1990.05.14', 'Female');\r\n" + 
    			"\r\n" + 
    			"insert into casting \r\n" + 
    			"values (17635, 3140, 'Main actor'),\r\n" + 
    			"(17635, 4095, 'Main actor'),\r\n" + 
    			"(22190, 3140, 'Main actor'),\r\n" + 
    			"(22190, 7347, 'Main actor'),\r\n" + 
    			"(32479, 1132, 'Main actor'),\r\n" + 
    			"(32479, 8103, 'Supporting actor'),\r\n" + 
    			"(83245, 4328, 'Main actor'),\r\n" + 
    			"(83245, 6810, 'Main actor'),\r\n" + 
    			"(11412, 7279, 'Main actor'),\r\n" + 
    			"(11412, 9091, 'Main actor');\r\n" + 
    			"\r\n" + 
    			"insert into genre \r\n" + 
    			"values ('Fantasy'),\r\n" + 
    			"('Romance'),\r\n" + 
    			"('Adventure'),\r\n" + 
    			"('Family'),\r\n" + 
    			"('Drama'),\r\n" + 
    			"('Action'),\r\n" + 
    			"('Mystery'),\r\n" + 
    			"('Thriller'),\r\n" + 
    			"('War');\r\n" + 
    			"\r\n" + 
    			"insert into movieGenre\r\n" + 
    			"values (17635, 'Fantasy'),\r\n" + 
    			"(17635, 'Romance'),\r\n" + 
    			"(22190, 'Fantasy'),\r\n" + 
    			"(22190, 'Adventure'),\r\n" + 
    			"(22190, 'Family'),\r\n" + 
    			"(32479, 'Drama'),\r\n" + 
    			"(83245, 'Action'),\r\n" + 
    			"(83245, 'Drama'),\r\n" + 
    			"(83245, 'Mystery'),\r\n" + 
    			"(83245, 'Thriller'),\r\n" + 
    			"(11412, 'Action'),\r\n" + 
    			"(11412, 'Drama'),\r\n" + 
    			"(11412, 'Thriller'),\r\n" + 
    			"(11412, 'War');\r\n" + 
    			"\r\n" + 
    			"insert into make\r\n" + 
    			"values(17635, 803),\r\n" + 
    			"(22190, 803),\r\n" + 
    			"(32479, 315),\r\n" + 
    			"(83245, 422),\r\n" + 
    			"(11412, 422);";
    }
    public static String createTable() {
    	return "create table director(\r\n" + 
    			"	directorID numeric(3),\r\n" + 
    			"	directorName varchar,\r\n" + 
    			"	dateOfBirth varchar(10),\r\n" + 
    			"	dateOfDeath varchar(10),\r\n" + 
    			"	primary key(directorID));\r\n" + 
    			"\r\n" + 
    			"create table actor(\r\n" + 
    			"	actorID numeric(4),\r\n" + 
    			"	actorName varchar,\r\n" + 
    			"	dateOfBirth varchar(10),\r\n" + 
    			"	dateOfDeath varchar(10),\r\n" + 
    			"	gender varchar(6),\r\n" + 
    			"	primary key(actorID));\r\n" + 
    			"\r\n" + 
    			"create table movie(\r\n" + 
    			"	movieID numeric(5),\r\n" + 
    			"	movieName varchar,\r\n" + 
    			"	releaseYear numeric(4),\r\n" + 
    			"	releaseMonth varchar(2),\r\n" + 
    			"	releaseDate numeric(2),\r\n" + 
    			"	publisherName varchar,\r\n" + 
    			"	avgRate real,\r\n" + 
    			"	primary key(movieID));\r\n" + 
    			"\r\n" + 
    			"create table award(\r\n" + 
    			"	awardID numeric(6),\r\n" + 
    			"	awardName varchar,\r\n" + 
    			"	primary key(awardID));\r\n" + 
    			"\r\n" + 
    			"create table genre(\r\n" + 
    			"	genreName varchar,\r\n" + 
    			"	primary key(genreName));\r\n" + 
    			"\r\n" + 
    			"create table movieGenre(\r\n" + 
    			"	movieID numeric(5),\r\n" + 
    			"	genreName varchar,\r\n" + 
    			"	primary key(movieID, genreName));\r\n" + 
    			"\r\n" + 
    			"create table movieObtain(\r\n" + 
    			"	movieID numeric(5),\r\n" + 
    			"	awardID numeric(6),\r\n" + 
    			"	year numeric(4),\r\n" + 
    			"	primary key(movieID, awardID));\r\n" + 
    			"\r\n" + 
    			"create table actorObtain(\r\n" + 
    			"	actorID numeric(4),\r\n" + 
    			"	awardID numeric(6),\r\n" + 
    			"	year numeric(4),\r\n" + 
    			"	primary key(actorID, awardID));\r\n" + 
    			"\r\n" + 
    			"create table directorObtain(\r\n" + 
    			"	directorID numeric(3),\r\n" + 
    			"	awardID numeric(6),\r\n" + 
    			"	year numeric(4),\r\n" + 
    			"	primary key(directorID, awardID));\r\n" + 
    			"\r\n" + 
    			"create table casting(\r\n" + 
    			"	movieID numeric(5),\r\n" + 
    			"	actorID numeric(4),\r\n" + 
    			"	role varchar,\r\n" + 
    			"	primary key(movieID, actorID));\r\n" + 
    			"\r\n" + 
    			"create table make(\r\n" + 
    			"	movieID numeric(5),\r\n" + 
    			"	directorID numeric(3),\r\n" + 
    			"	primary key(movieID, directorID));\r\n" + 
    			"\r\n" + 
    			"create table customerRate(\r\n" + 
    			"	customerID numeric(2),\r\n" + 
    			"	movieID numeric(5),\r\n" + 
    			"	rate numeric(1),\r\n" + 
    			"	primary key(customerID, movieID));\r\n" + 
    			"\r\n" + 
    			"create table customer(\r\n" + 
    			"	customerID numeric(2),\r\n" + 
    			"	customerName varchar,\r\n" + 
    			"	dateOfBirth varchar(10),\r\n" + 
    			"	gender varchar(6),\r\n" + 
    			"	primary key(customerID));";
    }
}