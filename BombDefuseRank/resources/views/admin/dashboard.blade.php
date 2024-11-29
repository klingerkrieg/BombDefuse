@extends('layouts.auth')

@section('title', 'Dashboard')

@section('content')
    <main>
        <div class="container">
            <div class="row justify-content-center">
                <div class="col-md-8">
                    <div class="card">
                        <div class="card-header d-flex justify-content-between align-items-baseline flex-wrap">
                            <h1 class="float-start">{{ __('Equipes') }}</h1>
                            <a href="{{route('new')}}" class="btn btn-primary float-end">Nova Equipe</a>
                        </div>
                        <div class="card-body">
                            @if (session('status'))
                                <div class="alert alert-success" role="alert">
                                    {{ session('status') }}
                                </div>
                            @endif
                            <ol>
                            @foreach ($teams as $team)
                                <li><a href="{{route('edit',$team['id'])}}">{{$team['time']}} {{$team['team_name']}} ({{number_format($team->score,2)}})</a></li>
                            @endforeach
                            </ol>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </main>
@endsection
