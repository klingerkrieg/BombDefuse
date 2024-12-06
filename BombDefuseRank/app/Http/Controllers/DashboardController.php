<?php

namespace App\Http\Controllers;

use App\Models\Event;
use App\Models\Member;
use App\Models\Team;
use App\Models\TeamQueue;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\Validator;


class DashboardController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('auth');
    }
    
    public function index() {
        #$defuseds = Team::with('members')->orderBy('time','asc')->where("exploded",0)->get();
        #$explodeds = Team::with('members')->orderBy('time','desc')->where("exploded",1)->get();
        
        #$teams = array_merge($defuseds->toArray(),$explodeds->toArray());

        $teams = Team::with('members')->where("event_name",env("APP_EVENT_NAME"))->get()->sortByDesc("score");
        
        return view('admin.dashboard', ["teams"=>$teams]);
    }

    public function new(){
        $equipesFila = TeamQueue::where('status', '!=', 1)
        ->orderBy('created_at', 'asc')
        ->get();

        return view("admin.team.form",[
            "team"=>new Team(),
            'equipesFila'=> $equipesFila
        ]);
    }

    protected function validator(array $data) {
        return Validator::make($data, [
            'team_name' => ['required', 'string', 'max:255'],
        ]);
    }

    public function save(Request $request){

        $this->validator($request->all())->validate();

        $data = $request->all();
        $data["event_name"] = env("APP_EVENT_NAME");
        $team = Team::create($data);
        
        $names = $request->get("name");
        foreach($names as $key=>$name){
            if ($name != null){
                Member::create([
                    "name"=>$name,
                    "age"=>$request->get("age")[$key],
                    "course"=>$request->get("course")[$key],
                    "participate"=>$request->get("participate")[$key],
                    "participateCount"=>$request->get("participateCount")[$key],
                    "team_id"=>$team->id
                ]);
            }
        }

        if($request->team_queue_id){
            $equipeFila = TeamQueue::find($request->team_queue_id);
            $equipeFila->status = 1;
            $equipeFila->save();
        }

        return redirect(route("edit",$team));
    }

    public function delete(Team $team){
        #remove todos
        foreach($team->members as $member){
            $member->delete();
        }
        foreach($team->events as $evt){
            $evt->delete();
        }
        $team->delete();
        return redirect(route('dashboard'));
    }

    public function foto(Team $team){
        return view("admin.team.foto", ["team"=>$team]);
    }

    public function edit(Team $team){
        return view("admin.team.form",["team"=>$team, 
                            "members"=>$team->members]);
    }

    public function update(Team $team, Request $request){
        $team->update($request->all());
        
        #remove todos
        foreach($team->members as $member){
            $member->delete();
        }

        #adiciona todos novamente
        $names = $request->get("name");
        foreach($names as $key=>$name){
            if ($name != null){
                Member::create([
                    "name"=>$name,
                    "age"=>$request->get("age")[$key],
                    "course"=>$request->get("course")[$key],
                    "participate"=>$request->get("participate")[$key],
                    "participateCount"=>$request->get("participateCount")[$key],
                    "team_id"=>$team->id
                ]);
            }
        }

        return redirect(route("edit",$team));
    }

}
