<?php

namespace App\Http\Controllers;

use App\Models\Member;
use App\Models\Team;
use Illuminate\Http\Request;

class WelcomeController extends Controller
{
    
    public function index() {
        $teams = Team::orderBy('time','desc')->get();
        return view('welcome', ["teams"=>$teams]);
    }

}
