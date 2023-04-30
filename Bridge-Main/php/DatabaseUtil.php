<?php
header( "Content-type:application/json" );

define( 'DATABASE_SERVER', 'remotemysql.com' );
define( 'DATABASE_USERNAME', 'm2H6QlRpXC' );
define( 'DATABASE_PASSWORD', '6RufKFvmGy' );
define( 'DATABASE_NAME', 'm2H6QlRpXC' );

$response = new stdClass();


if( $_SERVER["REQUEST_METHOD"] == "POST" ) {

    handleData( json_decode( $_POST["user"], false ) );

}


function getDatabaseHandler() {
    $database_connection = new mysqli( DATABASE_SERVER, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME );

    if( $database_connection->connect_error ) {
        if( !isset( $data ) ) {
            $data = new \stdClass();
        }
        $data->status = "failed";
                        sendResponse( $database_connection->connect_error, $data, json_encode( "{}" ) );

       

        return NULL;
    }
    return $database_connection;
}


function handleData( $data ) {

    $database_connection = getDatabaseHandler();

    if( $database_connection ) {
        $name = $data->name;
        $password = $data->password;
        $sql_query = "SELECT DISTINCT password FROM user1 WHERE username = '" . $name."'" ;

        $result = $database_connection->query( $sql_query );

        if( !isset( $data ) ) {
            $data = new \stdClass();
        }
        if ( !isset( $profile ) ) {
            $profile = new \stdClass();
        }

        if( $result ) {
            if( $result->num_rows > 0 ) {

                while ( $row = $result->fetch_assoc() ) {

                    $user_password = $row["password"];


                    if( strcmp( $user_password, $password ) == 0 ) {

                        $email = "";
                        $picture = "";

                        $data->status = "success";

                        $profile->name = $name;
                        $profile->email = $email;
                        $profile->picture = $picture;

                        sendResponse( "", $data, $profile );
                    } else {

                        $data->status = "failed";

                        sendResponse( "passwords donot match", $data, json_encode( "{}" ) );
                    }

                }

            }
            else{


                        $data->status = "failed";

                        sendResponse( "No player record found", $data, json_encode( "{}" ) );
            }
        } else {
            $data->status = "failed";

            sendResponse( "No player record found", $data, json_encode( "{}" ) );
        }


    } else {
         $data->status = "failed";

            sendResponse( "an unknown error has occured", $data, json_encode( "{}" ) );
       
    }


}

function sendResponse( $error, $data, $profile ) {
	global $response;
     $response->error = $error;
    $response->data = $data;
    $response->profile = $profile;
    echo json_encode($response);
}


?>
