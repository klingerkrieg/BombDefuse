@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card">
                <div class="card-header">
                    <h1 class="float-start">{{ __('BombDefuse RANK') }}</h1>
                    <a href="{{route('new')}}" class="btn btn-primary float-end">Nova equipe</a>
                </div>

                <div class="card-body">
                    @if (session('status'))
                        <div class="alert alert-success" role="alert">
                            {{ session('status') }}
                        </div>
                    @endif

                    <ol>
                    @foreach ($teams as $team)
                        <li><a href="{{route('edit',$team['id'])}}">{{$team['time']}} {{$team['team_name']}}</a></li>
                    @endforeach
                    </ol>

                </div>
            </div>
        </div>
    </div>
</div>
@endsection
